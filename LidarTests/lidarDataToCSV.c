nclude <stdio.h>
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
#define INDEX_START 0xA0
#define INDEX_END   0xF9

// Global:
static int fd;
static int angle;
static int packet_index;
static float rpm_speed;


int main (int argc, char *argv[]) {
    if ((fd = serialOpen (SERIAL_PORT, BAUD_RATE)) < 0) {
        fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
        return -1 ;
    }

    char buffer[1];
    char check_data[20];
    size_t speedSize, singleByte, checksumSize, dataSize;
    ssize_t bytesRead;
    int i, j, dataAvailable, currentCharacter;
    int calcCheck
    char speed[2] checksum[2];
    char data0[4], data1[4], data2[4], data3[4];
    char oneByte, beginByte;
    speedSize = sizeof(speed);
    singleByte = sizeof(beginByte);
    checksumSize = sizeof(checksum);
    dataSize = sizeof(data0);
    wiringPiSetup ()
     ;
    serialFlush(fd);


    if(!(dist2 & 0x80)) {
        dist2 = (dist2 & 0x3F) << 8;
        fprintf(writeFile, "%d,", (dist1 + dist2));
       fprintf(writeFile, "%d\n", (packet_index));
    }

    beginByte = 0;
    while(1) {
        if(beginByte == 0) {
            //starting bit
            bytesRead = read(fd, buffer, singleByte);
            if(buffer[0] == START_CODE) {
                beginByte = 1;
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
            //speed
            bytesRead = read(fd, speed, speedSize);

            //data 0, 1, 2, 3
            //0
            bytesRead = read(fd, data0, dataSize);
            //1
            bytesRead = read(fd, data1, dataSize);
            //2
            bytesRead = read(fd, data2, dataSize);
            //3
            bytesRead = read(fd, data3, dataSize);
       
            //checksum
            bytesRead = read(fd, checksum, checksumSize);

            //calculate checksum
            /////////////////////////////////////
            calcCheck = 0xFA + (packet_index << 8);
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
                printf("Successful reading! \n");
                exit(0);
            }
        }
    }
/*
    for (;;) {
        dataAvailable = serialDataAvail(fd);
        if (strcmp(current_buffer,old_buffer) != 0) {
            printf("%i Characters buffered: %s\n",j, current_buffer);
            strcpy(old_buffer, current_buffer);
        }
        serialFlush(fd);

        msleep(100);   // sleep 100 millisecs
    }
    */
    return 0 ;
}
