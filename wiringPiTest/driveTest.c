#include <stdio.h>
#include <wiringPi.h>
#include <errno.h>
#include "driveControl.h"

int main(int argc, char *argv[]) {
  float direction, forwardVelocity;

  if (argc != 3) {
    printf("usage: %s <direction> <forward_velocity>\n", argv[0] );
    return 1;
  }

  wiringPiSetup();

  if (configureDriveIO() != 0) {
    printf("%d\n", errno);
  }

  sscanf(argv[1], "%f", &direction);
  sscanf(argv[2], "%f", &forwardVelocity);

  printf("setDirectionVelocity(%f, %f);\n", direction, forwardVelocity);

  setDirectionVelocity(direction, forwardVelocity);

  delay(10000);

  return 1;
}