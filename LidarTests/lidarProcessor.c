#include "lidarReadings.h"
#include "lidarProcessor.h"
#include "redisFunctions.h"

void* processLidar(void* _lidar_data) {
  REVOLUTION_DATA* lidar_data = _lidar_data;
  int curr_rev_count = 1;
  int currDistances [NUM_READINGS];
  int i, loopCount;

  while (1) {
    while (lidar_data->revolutionCount < curr_rev_count) {
      //copy data to local
      for (i = 0; i < NUM_READINGS; i++) {
        currDistances[i] = lidar_data->distance[i];
      }

      if (!(loopCount % 4)) {
        redisPostReading(currDistances, NUM_READINGS);
      }

      //todo: run regression stuff, get location

      curr_rev_count = lidar_data->revolutionCount + 1;
      loopCount ++;
    }
  }
}
