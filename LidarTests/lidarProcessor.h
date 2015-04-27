/* Definitions for Hough transform */
#ifndef PI
#define PI 3.1415927
#endif

#define RANGE_THETA 360
#define RANGE_R 2500

#define ACCUM_PT(X, Y) accumulator[(X) + (Y) * RANGE_R]
#define RAD(THETA) ((THETA) * PI / 180)

typedef struct line {
  float r; //expressed in millimeters
  float theta; //expressed in radians
} line;

line* houghTransform(REVOLUTION_DATA* lidar_data);

void* processLidar(void* lidar_data);
