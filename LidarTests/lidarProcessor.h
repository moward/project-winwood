/* Definitions for Hough transform */
#ifndef PI
#define PI 3.1415927
#endif

#define RANGE_THETA 360
#define RANGE_R 2500

#define ACCUM_PT(X, Y) accumulator[(X) + (Y) * RANGE_R]
#define RAD(THETA) ((THETA) * PI / 180)

#define ANGLE_MATCH_THRESHOLD 15;

typedef struct line {
  double r; //expressed in millimeters
  double theta; //expressed in radians
} line;

//point-slope
typedef struct cartesian_line {
  double y; //expressed in millimeters
  double m;
} cartesian_line;

typedef struct position {
  double x;
  double y;
  double direction; //in degrees
} position;

line** houghTransform(REVOLUTION_DATA* lidar_data);

void* processLidar(void* lidar_data);
