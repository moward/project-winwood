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

  while (1) {
    //wait for new data
    while (lidar_data->revolutionCount < curr_rev_count);

    //copy data to local
    for (i = 0; i < NUM_READINGS; i++) {
      currDistances[i] = lidar_data->distance[i];
    }

    if (loopCount % 4 == 0) {
      //redisPostReading(currDistances, NUM_READINGS);
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
char* outputNetPBM(unsigned short int * accumulator){
  int i, length;
  char* buffer = malloc(RANGE_R * RANGE_THETA * 5;

  if (!buffer) {
    //could not allocate memory
    return NULL;
  }

  //file header
  length += sprintf(buffer + length, "P2\n%d %d\n255", RANGE_R, RANGE_THETA);

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

line** houghTransform(REVOLUTION_DATA* lidar_data) {
  int i, j, r;
  double rho, x, y, m, c, s;

  //hough accumulator
  unsigned short int accumulator[RANGE_R * RANGE_THETA];

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
        //get perpendicular slope
        m = tan(RAD(j + 90));
        //basic strategy is to convert to general linear form and use perpendicular distance formula
        //constant term of general linear form
        c = y - m * x;

        //radius to nearest point of perpendicular line
        s = fabs(c) / sqrt(m * m + 1);

        /*if ((int) s == 224 && j == 14) {
          printf("distance[%d] = %d\n", i, r);
          //printf(" = (%f, %f)\n", x, y);
          printf("ACCUM_PT(%d, %d), m = %f, c = %f, RAD(j + 90) = %f\n\n", ((int) s), j, m, c, RAD(j + 90));
        }*/

        //increment point corresponding to current line
        if (s < RANGE_R) {
          ACCUM_PT(((int) s), j)++;
        }
      }
    }
  }

  //todo: detect lines
  return NULL;
}

/*
 * Based on last given position and lines from hough transform, give the most
 * likely current
 * odometry
 */
int getRobotPosition(position* current, line** bounds) {
  int l1, l2, m1, m2, bestL2;
  double currError, dTheta, x, y, theta, m;
  double bestError = 365 * 365 * 2;
  cartesian_line l, m;

  line midLine1, midLine2;

  //get parallel lines
  l1 = l2 = 1;
  for (l2 = 1; l2 < 4; l2++) {
    //make m1 and m2 remaining
    m1 = (l2 + 1) % 3 + 1;
    m2 = l2 % 3 + 1;

    //add error of l1 and l2
    dTheta = fabs(bounds[l1]->theta - bounds[l2]->theta);
    if (dTheta > 180) {
      dTheta = 360 - dTheta;
    }
    currError = dTheta * dTheta;

    //add error of m1 and m2
    dTheta = fabs(bounds[m1]->theta - bounds[m2]->theta);
    if (dTheta > 180) {
      dTheta = 360 - dTheta;
    }
    currError += dTheta * dTheta;

    if (currError < bestError) {
      bestError = currError;
      bestL2 = l2;
    }
  }

  printf("bestError: %f\n", bestError);

  //find first line
  x = (bounds[l1].r * cos(RAD(bounds[l1].theta)) + bounds[l2].r * cos(RAD(bounds[l2].theta)))/2;
  y = (bounds[l1].r * sin(RAD(bounds[l1].theta)) + bounds[l2].r * sin(RAD(bounds[l2].theta)))/2;

  theta = ((bounds[l1].theta + bounds[l2].theta + 360) % 180) / 2;

  l.m = tan(RAD(theta));

  l.y = y - x * (l.m);

  //find second line
  x = (bounds[m1].r * cos(RAD(bounds[m1].theta)) + bounds[m2].r * cos(RAD(bounds[m2].theta)))/2;
  y = (bounds[m1].r * sin(RAD(bounds[m1].theta)) + bounds[m2].r * sin(RAD(bounds[m2].theta)))/2;

  theta = ((bounds[m1].theta + bounds[m2].theta + 360) % 180) / 2;

  m.m = tan(RAD(theta));

  m.y = y - x * (m.m);

  return 1;
}
