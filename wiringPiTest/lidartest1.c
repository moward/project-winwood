//Read in data from the lidar

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#define BAUD_RATE   115200
#define SERIAL_PORT "/dev/ttyAMA0" //COM2 = /dev/tty1
#define START_CODE  0xFA //start flag
#define INDEX_START 0xA0 //min possible value for index
#define INDEX_END   0xF9 //max possible value for index
#define WRITE_FILE  "lidarData.csv" // file to write to

// Global:
static int fd;
static int angle;
static int packet_index;
static float rpm_speed;

float computeRPMSpeed(char byte1, char byte2) {
  rpm_speed = (float)((byte1 | (byte2 << 8))) / 64.0;
  return rpm_speed;
}

int main (int argc, char *argv[]) {
    if ((fd = serialOpen (SERIAL_PORT, BAUD_RATE)) < 0) {
        printf ("Unable to open serial device\n") ;
        return -1 ;
    }
    FILE *writeFile;
    char buffer[1];
    char check_data[22];
    size_t checkSize;
    size_t speedSize, singleByte, checksumSize, dataSize;
    ssize_t bytesRead;
    int i, j, dataAvailable, currentCharacter;
    int calcCheck, packetCount;
    int dist1, dist2;
    char speed[2], checksum[2];
    char data0[4], data1[4], data2[4], data3[4];
    char oneByte, beginByte;
    speedSize = sizeof(speed);
    singleByte = sizeof(buffer);
    checksumSize = sizeof(checksum);
    dataSize = sizeof(data0);
    checkSize = sizeof(check_data);
    wiringPiSetup () ;
    serialFlush(fd);
    packetCount = 0;
    beginByte = 0;
    writeFile = fopen(WRITE_FILE, "w");
    while(1) {
        if(beginByte == 0) {
            //starting bit
            bytesRead = read(fd, buffer, singleByte);
            //printf("%02x ", buffer[0]);
            if(buffer[0] == START_CODE) {
                beginByte = 1;
                //printf("start\n");
            }
            else {
                beginByte = 0;
            }
        }
        else if(beginByte == 1) {
            //index
            bytesRead = read(fd, buffer, singleByte);
            if(buffer[0] >= INDEX_START && buffer[0] <= INDEX_END) {
                packet_index = buffer[0] - INDEX_START;
                beginByte = 2;
            }
            else {
                beginByte = 0;
            }
        }
        else if(beginByte == 2) {
            beginByte = 0;
            //speed
            bytesRead = read(fd, buffer, singleByte);
            speed[0] = buffer[0];
            bytesRead = read(fd, buffer, singleByte);
            speed[1] = buffer[0];

            //data 0, 1, 2, 3
            //0
            for(i = 0; i < 4; i++) {
              bytesRead = read(fd, buffer, singleByte);
              data0[i] = buffer[0];
              if(i == 0) dist1 = buffer[0];
              if(i == 1) dist2 = buffer[0];
            }
            if(!(dist2 & 0x80)) {
              dist2 = (dist2 & 0x3F) << 8;
              fprintf(writeFile, "%d,", (dist1 + dist2));
              fprintf(writeFile, "%d\n", (packet_index * 4));
            }
            //1
            for(i = 0; i < 4; i++) {
              bytesRead = read(fd, buffer, singleByte);
              data1[i] = buffer[0];
              if(i == 0) dist1 = buffer[0];
              if(i == 1) dist2 = buffer[0];
            }
            if(!(dist2 & 0x80)) {
              dist2 = (dist2 & 0x3F) << 8;
              fprintf(writeFile, "%d,", (dist1 + dist2));
              fprintf(writeFile, "%d\n", (packet_index * 4 + 1));
            }
            //2
            for(i = 0; i < 4; i++) {
              bytesRead = read(fd, buffer, singleByte);
              data2[i] = buffer[0];
              if(i == 0) dist1 = buffer[0];
              if(i == 1) dist2 = buffer[0];
            }
            if(!(dist2 & 0x80)) {
              dist2 = (dist2 & 0x3F) << 8;
              fprintf(writeFile, "%d,", (dist1 + dist2));
              fprintf(writeFile, "%d\n", (packet_index * 4 + 2));
            }
            //3
            for(i = 0; i < 4; i++) {
              bytesRead = read(fd, buffer, singleByte);
              data3[i] = buffer[0];
              if(i == 0) dist1 = buffer[0];
              if(i == 1) dist2 = buffer[0];
            }       
            if(!(dist2 & 0x80)) {
              dist2 = (dist2 & 0x3F) << 8;
              fprintf(writeFile, "%d,", (dist1 + dist2));
              fprintf(writeFile, "%d\n", (packet_index * 4 + 3));
            }
            //checksum
            bytesRead = read(fd, buffer, singleByte);
            checksum[0] = buffer[0];
            bytesRead = read(fd, buffer, singleByte);
            checksum[1] = buffer[0];

            //calculate checksum
            /////////////////////////////////////
            calcCheck = 0xFA + ((packet_index + INDEX_START)  << 8);
            calcCheck = (calcCheck << 1) + speed[0] + (speed[1] << 8);
            calcCheck = (calcCheck << 1) + data0[0] + (data0[1] << 8);
            calcCheck = (calcCheck << 1) + data0[2] + (data0[3] << 8);
            calcCheck = (calcCheck << 1) + data1[0] + (data1[1] << 8);
            calcCheck = (calcCheck << 1) + data1[2] + (data1[3] << 8);
            calcCheck = (calcCheck << 1) + data2[0] + (data2[1] << 8);
            calcCheck = (calcCheck << 1) + data2[2] + (data2[3] << 8);
            calcCheck = (calcCheck << 1) + data3[0] + (data3[1] << 8);
            calcCheck = (calcCheck << 1) + data3[2] + (data3[3] << 8);
            calcCheck = (calcCheck & 0x7FFF) + (calcCheck >> 15);
            calcCheck = calcCheck & 0x7FFF;
            //////////////////////////////////////
            if(calcCheck == (checksum[0] + (checksum[1] << 8))) {
               // printf("Successful reading! \n");
                //comma ascii is 44
                packetCount++;
                //exit(0);
            }
            else {
                printf("Checksum is wrong! \n");
                packetCount++;
                printf("Calculated is %x but should be %x \n",calcCheck, (checksum[0] + (checksum[1] << 8)));
                printf("1:fa    2:%x    3:%x    4:%x\n",packet_index + INDEX_START, speed[0], speed[1]);
                printf("5:%x   6:%x    7:%x    8:%x\n", data0[0],data0[1],data0[2],data0[3]);
                printf("9:%x    10:%x    11:%x    12:%x\n", data1[0],data1[1],data1[2],data1[3]);
                printf("13:%x    14:%x    15:%x    16:%x\n", data2[0],data2[1],data2[2],data2[3]);
                printf("17:%x    18:%x    19:%x    20:%x\n", data3[0],data3[1],data3[2],data3[3]);
                exit(0);
            }
            if(packetCount == 180) {
              printf("180 packets received!     ");
              computeRPMSpeed(speed[0], speed[1]);
              printf("Ending RPM: %f\n", rpm_speed);
              exit(0);
            }
        }
        else exit(0);
    }
/*
    for (;;) {
        dataAvailable = serialDataAvail(fd);
        if (strcmp(current_buffer,old_buffer) != 0) {
            printf("%i Characters buffered: %s\n",j, current_buffer);
            strcpy(old_buffer, current_buffer);
        }
        serialFlush(fd);

        nt)(dist1 + dist2));
            fprintf(writeFile, "%msleepi100);   // sleep 100 millisecs
    }
    */
    return 0 ;
}
