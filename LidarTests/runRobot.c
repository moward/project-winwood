/*
* File: runLidarThreads.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <math.h>
#include "hiredis.h"
#include "lidarReadings.h"
#include "lidarProcessor.h"
#include "redisFunctions.h"
#include "driveCommand.h"
#include "closedLoop.h"
#include "../wiringPiTest/driveControl.h"

#define NUM_THREADS 4

#define BOT_CENTER_OFFSET 0.07

pthread_mutex_t waypointListLock;
LINKEDNODE* waypointList;

int notDead = 1;

double distance(position* p1, position* p2) {
  double dx = 0;
  double dy = 0;
  dx = p1->x - p2->x;
  dy = p1->x - p2->x;
  return sqrt(dx * dx + dy * dy);
}

int main(int argc, char *argv[]) {
  LINKEDNODE* nextNode;

  //verify number of arguments
  if (argc != 3) {
    printf("usage: runRobot <redis_IP_Address> <robot_name>\n");
    exit(1);
  }

  //open Redis
  if(!openRedisConnection(argv[1], argv[2])) {
    printf("Error starting Redis\n");
    exit(1);
  }

  printf("Starting robot\n");
  redisLog("Starting robot");

  //Configure wiringPi
  wiringPiSetup();

  printf("wiringpi complete\n");

  if (configureDriveIO() != 0) {
    printf("Could not configure wiringPiIO\n");
  }

  //create threads
  pthread_t threads[NUM_THREADS];
  REVOLUTION_DATA total_data;
  total_data.revolutionCount = 0;
  total_data.errorCount = 0;
  //int oldRevCount = 0;
  printf("creating thread\n");
  pthread_create(&threads[0], NULL, readData, &total_data);
  pthread_create(&threads[1], NULL, processLidar, &total_data);
  pthread_create(&threads[2], NULL, commandListen, NULL);
  pthread_create(&threads[3], NULL, driver, NULL);
  /*while(total_data.revolutionCount < 50) {
    if(oldRevCount != total_data.revolutionCount) {
        oldRevCount = total_data.revolutionCount;
        printf("Dist: %d    Angle: %d \n", total_data.distance[0], total_data.angle[0]);
    }
  }*/

  redisLog("Terminating robot");

  return 0;
}

void driver(void* nothing) {
  waypointList = malloc(sizeof(LINKEDNODE));
  waypointList->behav = NONE;
  waypointList->next = NULL;
  waypointList->node = malloc(sizeof(NODE));
  waypointList->node->pos.x = 0;
  waypointList->node->pos.y = 0;

  while(notDead) {
    nextNode = waypointList;

    pthread_mutex_lock(&waypointListLock);

    //if not within certain threshhold of destination
    if (nextNode != NULL && distance(&currPos, &(nextNode->node->pos)) < AT_DESTINATION_THRESH) {
      printf("Distance: %f\n", distance(&currPos, &(nextNode->node->pos)));
      //insert driving code here
      closedLoopControlToNextPoint(&currPos, nextNode);
    } else if (nextNode != NULL) {
      printf("reached next node\n");
      //you've reached next node, pop item
      waypointList = waypointList->next;
      free(nextNode);
    } if (nextNode == NULL) {
      printf("at destination\n");
      stopDriving();
    }

    pthread_mutex_unlock(&waypointListLock);
  }
}

void closedLoopControlToNextPoint(position* currPos, LINKEDNODE* nextPoint) {
  double center_x, botDirection, directionDiff, direction;
  position relativeNextPoint;
  position* nextPos = &(nextPoint->node->pos);

  botDirection = RAD(currPos->direction);

  //if facing wrong direction, pivot
  directionDiff = nextPos->direction - currPos->direction;
  if (directionDiff > 180) {
    directionDiff -= 360;
  } else if (directionDiff < -180) {
    directionDiff += 360;
  }

  //pivot calls
  if (directionDiff > 80) {
    pivot(-0.3);
  } else if (directionDiff < -80) {
    pivot(0.3);
  } else {
    //drive forward-ish
    //rotate relativeNextPoint
    relativeNextPoint.x = (nextPos->x - currPos->x) * cos(-botDirection)
        - (nextPos->y - currPos->y) * sin(-botDirection);
    relativeNextPoint.y = (nextPos->x - currPos-> x) * sin(-botDirection)
        + (nextPos->y - currPos->y) * cos(-botDirection);
    
    if (relativeNextPoint.x > 0.05) {
      //calculate circle to get to next point
      center_x = (relativeNextPoint.x * relativeNextPoint.x
          + relativeNextPoint.y * relativeNextPoint.y)
          / (2 * relativeNextPoint.x);

      direction = halfWheelDistance / center_x;
    } else {
      direction = 0;
    }

    printf("Calculated circle_x to be %f\n", center_x);

    setDirectionVelocity((float) (direction + BOT_CENTER_OFFSET), 0.2);
  }
}
