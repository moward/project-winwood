/* Definitions for Hough transform */
#ifndef PI
#define PI 3.1415927
#endif

#define RANGE_THETA 360
#define RANGE_R 1300

#define ACCUM_PT(X, Y) accumulator[(X) + (Y) * RANGE_R]
#define RAD(THETA) ((THETA) * PI / 180)

#define ANGLE_MATCH_THRESHOLD 15

#define LINE_X(LINE) (bounds[LINE]->r * cos(RAD(bounds[LINE]->theta)))
#define LINE_Y(LINE) (bounds[LINE]->r * sin(RAD(bounds[LINE]->theta)))

typedef struct line {
  int r; //expressed in millimeters
  int theta; //expressed in radians
} line;

//point-slope
typedef struct cartesian_line {
  double y; //expressed in millimeters
  double m;
} cartesian_line;

#ifndef POSITION
typedef struct position {
  double x;
  double y;
  double direction; //in degrees
} position;
#endif

unsigned short int* houghTransform(REVOLUTION_DATA* lidar_data);

char* outputNetPBM(unsigned short int* accumulator);

line** findLines(unsigned short int* houghSpace);

void* processLidar(void* lidar_data);

int getRobotPosition(position* current, line** bounds);

extern position currPos;
