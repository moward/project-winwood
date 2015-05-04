#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <errno.h>
#include "hiredis.h"
#include "lidarReadings.h"
#include "lidarProcessor.h"
#include "redisFunctions.h"
#include "closedLoop.h"
#include "driveCommand.h"
#include "../wiringPiTest/driveControl.h"

void redisSubscribeCallback(redisReply *reply) {
  float x, y;
  char* replyStr;
  if (reply != NULL
    && reply->type == REDIS_REPLY_ARRAY
    && reply->elements >= 2
    && reply->element[2]->type == REDIS_REPLY_STRING) {

    replyStr = reply->element[2]->str;
    if(sscanf(replyStr, "%f,%f", &x, &y) != 2) {
      printf("Illegal driveCommand\n");
      return;
    }

    printf("1\n");

    pthread_mutex_lock(&waypointListLock);

    printf("2\n");

    //add drive goal
    waypointList = malloc(sizeof(LINKEDNODE));
    waypointList->behav = NONE;
    waypointList->next = NULL;
    waypointList->node = malloc(sizeof(NODE));
    waypointList->node->pos.x = x;
    waypointList->node->pos.y = y;

    printf("3\n");

    pthread_mutex_unlock(&waypointListLock);

    printf("4\n");

  } else {
    printf("Unexpected reply structure\n");
  }
}

void *commandListen(void* nothing) {
  //set up wiringpi

  redisSubscribe(&redisSubscribeCallback);
}
