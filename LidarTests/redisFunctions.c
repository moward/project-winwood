#ifndef __HIREDIS_AE_H__
#define __HIREDIS_AE_H__
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis.h>

#include "redisFunctions.h"

char* robotName = NULL;
redisContext *c = NULL;

//log an arbitrary string
//should end with a newline by convention
void redisLog(char *message) {
  redisReply *reply;
  char buff[1024];

  sprintf(buff, "%s: %s", robotName, message);

  reply = redisCommand(c, "PUBLISH stdlog %b\n", buff, strlen(buff));
  //todo: remove
  printf("PUBLISH: %lld\n", reply->integer);

  freeReplyObject(reply);
}

//Post the robot's position to redis
void redisSetPosition(char *message) {

}

//Post robot's reading
void redisPostReading(int* distances) {

}

int openRedisConnection(char* serverIp, char* _robotName) {
  //open redis connection
  struct timeval timeout = { 1, 500000 }; // 1.5 seconds
  c = redisConnectWithTimeout(serverIp, 6379, timeout);
  if (c == NULL || c->err) {
    if (c) {
      printf("Connection error: %s\n", c->errstr);
      redisFree(c);
    } else {
      printf("Connection error: can't allocate redis context\n");
    }
    return 0;
  }

  robotName = _robotName;

  return 1;
}
 