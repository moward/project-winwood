#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "lidarReadings.h"
#include "lidarProcessor.h"
#include "redisFunctions.h"

void* processLidar(void* _lidar_data) {
  REVOLUTION_DATA* lidar_data = _lidar_data;
  int curr_rev_count = 1;
  int currDistances [NUM_READINGS];
  int i, loopCount;
  line** lines;
  unsigned short int* houghSpace;

  char buffer[128];

  position currPos;

  currPos.x = 0;
  currPos.y = 0;
  currPos.direction = 0;

  while (1) {
    //wait for new data
    while (lidar_data->revolutionCount < curr_rev_count);

    //copy data to local
    for (i = 0; i < NUM_READINGS; i++) {
      currDistances[i] = lidar_data->distance[i];
    }

    if (loopCount % 4 == 0) {

      houghSpace = houghTransform(lidar_data);

      lines = findLines(houghSpace);

      free(houghSpace);

      getRobotPosition(&currPos, lines);

      sprintf(buffer, "Robot position: (%.2f, %.2f) Angle: %.2f\n",
          currPos.x, currPos.y, currPos.direction);

      redisLog(buffer);

      //post readings to redis
      redisPostReading(currDistances, NUM_READINGS);
      //printf("Posted reading to Redis server!\n");
    }

    //todo: run regression stuff, get location
    curr_rev_count = lidar_data->revolutionCount + 1;
    loopCount ++;
  }
}

/* Creates a PGB Grayscale bitmap file from the given accumulator 
 * Note: result must be freed afterwards
 */
char* outputNetPBM(unsigned short int* accumulator) {
  int i, length;
  char* buffer = malloc(RANGE_R * RANGE_THETA * 5);
  if (!buffer) {
    //could not allocate memory
    return NULL;
  }

  //file header
  length = sprintf(buffer, "P2\n%d %d\n255", RANGE_R, RANGE_THETA);

  for (i = 0; i < RANGE_R * RANGE_THETA; i++) {
    if (i % RANGE_R == 0) {
      length += sprintf(buffer + length, "\n", accumulator[i]);
    } else {
      length += sprintf(buffer + length, " ", accumulator[i]);
    }
    length += sprintf(buffer + length, "%3hu", accumulator[i]);
  }

  length += sprintf(buffer + length, "\n", accumulator[i]);

  return buffer;
}

unsigned short int* houghTransform(REVOLUTION_DATA* lidar_data) {
  int i, j, r;
  double rho, x, y, m, c, s;
  line** lines;

  unsigned short int* accumulator
    = malloc(RANGE_R * RANGE_THETA * sizeof(unsigned short int));

  if (accumulator == NULL) {
    //could not allocate memory
    return NULL;
  }

  memset(accumulator, 0, RANGE_R * RANGE_THETA * sizeof(unsigned short int));

  //iterate over each of 360 readings
  for (i = 0; i < NUM_READINGS; i++) {
    rho = RAD(i);
    r = lidar_data->distance[i];

    //take only valid values
    if (r > 0) {
      x = r * cos(rho);
      y = r * sin(rho);

      //for j in [0, 180], add line through point to accumulator
      for (j = 0; j < RANGE_THETA; j++) {
        //only consider angles that will orient correctly with the current point
        if ((j - i + 450) % 360 < 180) {
          //get perpendicular slope
          m = tan(RAD(j + 90));
          //basic strategy is to convert to general linear form and use
          //perpendicular distance formula
          //constant term of general linear form
          c = y - m * x;

          //radius to nearest point of perpendicular line
          s = fabs(c) / sqrt(m * m + 1);

          //increment point corresponding to current line
          if (s < RANGE_R) {
            ACCUM_PT((int) s, j)++;
          }
        }
      }
    }
  }

  /*pgmOutput = outputNetPBM(accumulator);
  printf("%s", pgmOutput);
  free(pgmOutput);*/
  
  return accumulator;
}

line** findLines(unsigned short int* houghSpace) {
  int i, j, k, theta;
  int brightest, brightestR, brightestTheta;
  unsigned short int* accumulator = houghSpace;

  //allocate line structs
  line** lines = malloc(4 * sizeof(line));

  if (lines == NULL) {
    //could not allocate memory
    return NULL;
  }

  for (i = 0; i < 4; i++) {
    lines[i] = malloc(sizeof(line));
    if (lines[i] == NULL) {
      //could not allocate memory
      return NULL;
    }
  }

  brightest = 0;
  brightestR = brightestTheta = -1;

  //start by finding the brightest point
  for (i = 0; i < RANGE_R; i++) {
    for (j = 0; j < RANGE_THETA; j++) {
      if (ACCUM_PT(i, j) > brightest) {
        brightest = ACCUM_PT(i, j);
        brightestR = i;
        brightestTheta = j;
      }
    }
  }

  if (brightestR == -1 || brightestTheta == -1) {
    printf("houghTransform: All data zero\n");
    return NULL;
  }

  lines[0]->r = brightestR;
  lines[0]->theta = brightestTheta;

  //then, look for additional points at approximately 90 degrees away
  for (i = 1; i < 4; i++) {
    brightest = brightestR = brightestTheta = 0;
    for (j = 0; j < RANGE_R; j++) {
      //look with range of 10, i.e. 80 to 100
      for (k = lines[0]->theta + 90 * i - 10; k < lines[0]->theta + 90 * i + 10; k++) {
        theta = k % RANGE_THETA;
        if (ACCUM_PT(j, theta) > brightest) {
          brightest = ACCUM_PT(j, theta);
          brightestR = j;
          brightestTheta = theta;
        }
      }
    }

    lines[i]->r = brightestR;
    lines[i]->theta = brightestTheta;
  }

  return lines;
}

void considerDirection (double direction, double* leastError, double oldDirection,
    double* bestDirection) {
  double error = fabs(direction - oldDirection);
  if (error > 180) {
    error = 360 - error;
  }
  if (error < *leastError) {
    *bestDirection = direction;
    *leastError = error;
  }
}

/*
 * Based on last given position and lines from hough transform, give the most
 * likely current
 * odometry
 */
int getRobotPosition(position* current, line** bounds) {
  int l1, l2, m1, m2, bestL2;
  double currError, newDirection, dTheta, theta, bestDirection, leastError, tempX;
  double bestError = 365 * 365 * 2;
  position l, m;

  line midLine1, midLine2;

  //get parallel lines
  l1 = 0;
  l2 = 2;
  m1 = 1;
  m2 = 3;

  //find first line
  l.x = (bounds[l1]->r * cos(RAD(bounds[l1]->theta))
      + bounds[l2]->r * cos(RAD(bounds[l2]->theta)))/2;
  l.y = (bounds[l1]->r * sin(RAD(bounds[l1]->theta))
      + bounds[l2]->r * sin(RAD(bounds[l2]->theta)))/2;

  //get arithmetic average of angles
  l.direction = bounds[l1]->theta % 180 + bounds[l2]->theta % 180;

  if(abs(bounds[l1]->theta % 180 - bounds[l2]->theta % 180) > 90) {
    l.direction -= 90;
  }

  l.direction /= 2;

  //find second line
  m.x = (bounds[m1]->r * cos(RAD(bounds[m1]->theta))
      + bounds[m2]->r * cos(RAD(bounds[m2]->theta)))/2;
  m.y = (bounds[m1]->r * sin(RAD(bounds[m1]->theta))
      + bounds[m2]->r * sin(RAD(bounds[m2]->theta)))/2;

  //get arithmetic average of angles
  m.direction = bounds[m1]->theta % 180 + bounds[m2]->theta % 180;

  if(abs(bounds[m1]->theta % 180 - bounds[m2]->theta % 180) > 90) {
    m.direction -= 90;
  }

  m.direction /= 2;

  //calculate center of field
  current->x = m.x + l.x;

  current->y = m.y + l.y;

  //Transform step 1: negate x and y
  current->x = -current->x;
  current->y = -current->y;

  //Transform step 2: determine most probable robot direction using the two
  //line directions
  leastError = 360 * 360;

  considerDirection (l.direction, &leastError, current->direction, &bestDirection);
  considerDirection (m.direction, &leastError, current->direction, &bestDirection);
  considerDirection (l.direction + 180, &leastError, current->direction, &bestDirection);
  considerDirection (m.direction + 180, &leastError, current->direction, &bestDirection);

  current->direction = 360 - bestDirection;

  //rotate by new direction
  tempX = current->x;

  current->x = current->x * cos(RAD(current->direction))
      - current->y * sin(RAD(current->direction));
  current->y = current->y * cos(RAD(current->direction))
      + tempX * sin(RAD(current->direction));

  return 1;
}
