#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lidarReadings.h"
#include "lidarProcessor.h"

int main() {
  REVOLUTION_DATA data;
  FILE *fp;
  int r, theta;

  fp = fopen("../../lidarData.csv", "r");

  if (fp == NULL) {
    perror ( "Unable to open file" );
    exit ( EXIT_FAILURE );
  }

  //set array to zero default
  memset(data.distance, 0, NUM_READINGS * sizeof(int));

  while (!feof(fp)) {

    fscanf(fp, "%d,%d", &r, &theta);
    data.distance[theta] = r;
  }

  houghTransform(&data);

  fclose(fp);
  return 0;
}
