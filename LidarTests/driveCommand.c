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
#include "../wiringPiTest/driveControl.h"

void redisSubscribeCallback(redisReply *reply) {
  float direction, magnitude;
  char* replyStr;
  if (reply != NULL
    && reply->type == REDIS_REPLY_ARRAY
    && reply->elements >= 2
    && reply->element[2]->type == REDIS_REPLY_STRING) {

    replyStr = reply->element[2]->str;
    if(sscanf(replyStr, "%f,%f", &direction, &magnitude) != 2) {
      printf("Illegal driveCommand\n");
      return;
    }
    //setDirectionVelocity(direction, magnitude);
  } else {
    printf("Unexpected reply structure\n");
  }
}

void *commandListen(void* nothing) {
  //set up wiringpi

  redisSubscribe(&redisSubscribeCallback);
}
