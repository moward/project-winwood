#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <errno.h>
#include "driveControl.h"
#include "pinAssignments.h"

int configureDriveIO () {
  pinMode(LEFT_WHEEL_IN1, OUTPUT);
  pinMode(LEFT_WHEEL_IN2, OUTPUT);
  pinMode(RIGHT_WHEEL_IN1, OUTPUT);
  pinMode(RIGHT_WHEEL_IN2, OUTPUT);
  softPwmCreate (LEFT_WHEEL_PWM, 0, 100);
  softPwmCreate (RIGHT_WHEEL_PWM, 0, 100);
  return 0;
}

int setDirectionVelocity (float direction, float forwardVelocity) {
  // boundary checks
  if (direction < -1 || direction > 1) {
    return 0;
  } else if (forwardVelocity < -1 || forwardVelocity > 1) {
    return 0;
  }
  //set wheel velocities
  //
  //default to edge case of straight travel
  float leftVelocity = forwardVelocity / 2;
  float rightVelocity = forwardVelocity / 2;
  
  //handle general case
  if (fabs(direction) > 0.05) {
      float radius = halfWheelDistance / fabs(direction);

      //make sure direction and radius have same sign
      if (direction < 0) {
          radius *= -1;
      }

      float omega = forwardVelocity / radius;
      
      leftVelocity = omega * (radius + halfWheelDistance) / 2;
      rightVelocity = omega * (radius - halfWheelDistance) / 2;
  }

  //console.log('Velocities: (' + leftVelocity + ', '
  //    + rightVelocity + ')');
  //    
  
  //set H-bridge values
  if (fabs(leftVelocity) < 0.05) {
    //brake
    digitalWrite(LEFT_WHEEL_IN1, 0);
    digitalWrite(LEFT_WHEEL_IN2, 0);
  } else if (leftVelocity > 0) {
    //forward
    digitalWrite(LEFT_WHEEL_IN1, 1);
    digitalWrite(LEFT_WHEEL_IN2, 0);
  } else if (leftVelocity < 0) {
    //reverse
    digitalWrite(LEFT_WHEEL_IN1, 0);
    digitalWrite(LEFT_WHEEL_IN2, 1);
  }

  if (fabs(rightVelocity) < 0.05) {
    //brake
    digitalWrite(RIGHT_WHEEL_IN1, 0);
    digitalWrite(RIGHT_WHEEL_IN2, 0);
  } else if (rightVelocity > 0) {
    //forward
    digitalWrite(RIGHT_WHEEL_IN1, 1);
    digitalWrite(RIGHT_WHEEL_IN2, 0);
  } else if (rightVelocity < 0) {
    //reverse
    digitalWrite(RIGHT_WHEEL_IN1, 0);
    digitalWrite(RIGHT_WHEEL_IN2, 1);
  }

  //printf("(%f,%f)\n", leftVelocity, rightVelocity);

  // printf("softPwmWrite(LEFT_WHEEL_PWM, %d);\nsoftPwmWrite(RIGHT_WHEEL_PWM, %d);\n",
  //   abs((int) (leftVelocity * 100)),
  //   abs((int) (rightVelocity * 100)));

  softPwmWrite(LEFT_WHEEL_PWM, abs((int) (leftVelocity * 100)));
  softPwmWrite(RIGHT_WHEEL_PWM, abs((int) (rightVelocity * 100)));

  return 1;
}

int stopDriving() {
  digitalWrite(LEFT_WHEEL_IN1, 0);
  digitalWrite(LEFT_WHEEL_IN2, 0);
  digitalWrite(RIGHT_WHEEL_IN1, 0);
  digitalWrite(RIGHT_WHEEL_IN2, 0);
  softPwmWrite(LEFT_WHEEL_PWM, 0);
  softPwmWrite(RIGHT_WHEEL_PWM, 0);
}

int pivot(double speed) {
  //boundary checks
  if (fabs(speed) > 1) {
    return 0;
  }

  //set motor direciton
  if (speed > 0) {
    digitalWrite(RIGHT_WHEEL_IN1, 0);
    digitalWrite(RIGHT_WHEEL_IN2, 1);
    digitalWrite(LEFT_WHEEL_IN1, 1);
    digitalWrite(LEFT_WHEEL_IN2, 0);
  } else {
    digitalWrite(RIGHT_WHEEL_IN1, 1);
    digitalWrite(RIGHT_WHEEL_IN2, 0);
    digitalWrite(LEFT_WHEEL_IN1, 0);
    digitalWrite(LEFT_WHEEL_IN2, 1);
  }

  //set speed
  softPwmWrite(LEFT_WHEEL_PWM, abs((int) (speed * 100)));
  softPwmWrite(RIGHT_WHEEL_PWM, abs((int) (speed * 100)));
}
