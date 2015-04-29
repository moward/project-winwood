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
  sscanf(reply->str, "%f,%f", &direction, &magnitude);
  setDirectionVelocity(direction, magnitude);
}

void *commandListen(void* nothing) {
  redisSubscribe(redisSubscribeCallback);
}
