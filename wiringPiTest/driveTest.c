#include <stdio.h>
#include <wiringPi.h>
#include <errno.h>
#include "driveControl.h"

int main(int argc, char *argv[]) {
  float direction, forwardVelocity;

  wiringPiSetup();

  if (configureDriveIO() != 0) {
    printf("%d\n", errno);
  }

  printf("Enter <direction> <forwardVelocity>\n");
  while (scanf("%f %f", &direction, &forwardVelocity)){
    setDirectionVelocity(direction, forwardVelocity);
  }
  return 1;
}