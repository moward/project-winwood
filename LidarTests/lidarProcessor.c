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

line* houghTransform(REVOLUTION_DATA* lidar_data) {
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
        //find perpendicular distance
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

    //printf("Finished reading %d\n", i);
  }

  //print Portable GrayMap
  //header information
  printf("P2\n%d %d\n255", RANGE_R, RANGE_THETA);

  for (i = 0; i < RANGE_R * RANGE_THETA; i++) {
    if (i % RANGE_R == 0) {
      putc('\n', stdout);
    } else {
      putc(' ', stdout);
    }
    printf("%3hu", accumulator[i]);
  }

  putc('\n', stdout);

  //todo: detect lines
  return NULL;
}

