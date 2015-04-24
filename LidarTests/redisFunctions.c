#ifndef __HIREDIS_AE_H__
#define __HIREDIS_AE_H__

#include <string.h>
#include <sys/types.h>
#include <ae.h>
#include <hiredis.h>
#include <async.h>

#include "redisFunctions.h"

char* robotName = NULL;
redisAsyncContext *c = NULL;

//log an arbitrary string
//should end with a newline by convention
void redisLog(char *message) {
  redisCommand(c, "PUBLISH stdlog \"%s: %s\"", message);
}

//Post the robot's position to redis
void redisSetPosition(char *message) {

}

//Post robot's reading
void redisPostReading(int* distances) {

}

void openRedisConnection(char* serverIp, char* _robotName) {
  //open redis connection
  redisContext *c = redisConnect(serverIp, 6379);
  if (c != NULL && c->err) {
      printf("Error: %s\n", c->errstr);
      // handle error
  }

  robotName = _robotName;
}
