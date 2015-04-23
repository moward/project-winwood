/* 
  File name: lidarReadings.h
*/
#ifndef lidarReadings
#define lidarReadings
#define BAUD_RATE    115200
#define SERIAL_PORT  "/dev/ttyAMA0" //COM2 = /dev/tty1
#define START_CODE   0xFA //start flag
#define INDEX_START  0xA0 //min possible value for index
#define INDEX_END    0xF9 //max possible value for index
#define WRITE_FILE   "lidarData.csv" // file to write to
#define NUM_READINGS 360

typedef struct rev_data
{
    int revolutionCount;
    int errorCount;
    int distance [NUM_READINGS];
    int angle [NUM_READINGS];
} REVOLUTION_DATA;

float computeRPMSpeed(char byte1, char byte2);
void *readData ();
#endif
