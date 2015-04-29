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
  freeReplyObject(reply);
}

//Post the robot's position to redis
void redisSetPosition(char *message) {

}

//Post robot's reading
void redisPostReading(int* distances, int length) {
  redisReply *reply;
  //first, generate list of space-separated distances as a string
  char distanceList[length * 5];
  char pushCommand[length * 5 + 30];
  int i;
  char* cursor = distanceList;

  for (i = 0; i < length; i++) {
    sprintf(cursor, "%d ", distances[i]);
    cursor += strlen(cursor);
  }

  //increment counter
  reply = redisCommand(c, "INCR %s.distancePosts", robotName);
  freeReplyObject(reply);

  //clear previous reading data
  reply = redisCommand(c, "LTRIM %s.distances 0 0", robotName);
  freeReplyObject(reply);

  //generate push command to buffer
  sprintf(pushCommand, "LPUSH %s.distances %s", robotName, distanceList);
  reply = redisCommand(c, pushCommand);
  //printf("(%lld) %s\n", reply->integer, pushCommand);

  freeReplyObject(reply);
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
void redisSubscribe(void (*fun)(redisReply*)) {
  redisReply *reply = redisCommand(c, "SUBSCRIBE driveCommand");
  freeReplyObject(reply);
  while(redisGetReply(c,(void **) &reply) == REDIS_OK) {
    fun(reply);
    freeReplyObject(reply);
  }
}
