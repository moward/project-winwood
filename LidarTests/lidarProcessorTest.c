#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lidarReadings.h"
#include "lidarProcessor.h"

int main(int argc, char *argv[]) {
  REVOLUTION_DATA data;
  FILE *fp;
  int r, theta, i;
  unsigned short int* houghSpace;
  //char * netPBMFile;

  if (argc != 2) {
    printf("Need to provide filename\n");
    return 1;
  }

  fp = fopen(argv[1], "r");

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

  houghSpace = houghTransform(&data);

  /*netPBMFile = outputNetPBM(houghSpace);
  free(houghSpace);
  printf("%s", netPBMFile);
  free (netPBMFile);*/

  line** lines = findLines(houghSpace);

  for (i = 0; i < 4; i++) {
    printf("found line[%d] at (%d, %d)\n", i, lines[i]->r, lines[i]->theta);
  }

  position currPos;

  currPos.x = 0;
  currPos.y = 0;
  currPos.direction = 0;

  getRobotPosition(&currPos, lines);

  printf("%f, %f, %f\n", currPos.x, currPos.y, currPos.direction);

  fclose(fp);
  return 0;
}
