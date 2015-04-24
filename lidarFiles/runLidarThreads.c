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

#include <hiredis.h>
#include <async.h>
#include <adapters/ae.h>

#include "lidarReadings.h"

//number of threads not counting current
#define NUM_THREADS 4

redisAsyncContext *c;

int main(int argc, char *argv[]) {
  if(argc != 2) {
    printf("Usage: runRobot <REDIS_SERVER_IP>\n");
    exit(0);
  }

  //connect to redis
  c = redisAsyncConnect(argv[1], 6379);
  if (c->err) {
      printf("Error: %s\n", c->errstr);
      exit(0);
  }

  wiringPiSetup ();
  pthread_t threads[NUM_THREADS];
  REVOLUTION_DATA total_data
  total_data.revolutionCount = 0;
  total_data.errorCount = 0;
  int oldRevCount = 0;
  printf("creating thread\n");
  pthread_create(&threads[0], NULL, readData, (void *) &total_data);
  while(total_data.revolutionCount < 50) {
    if(oldRevCount != total_data.revolutionCount) {
        oldRevCount = total_data.revolutionCount;
        printf("Dist: %d    Angle: %d \n", total_data.distance[0], total_data.angle[0]);
    }
  }
}
