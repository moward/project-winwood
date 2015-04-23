lidarThreads: lidarReadings.c lidarReadings.h runLidarThreads.c
	gcc -c lidarReadings.c -lpthread -lwiringPi -lm
	gcc -Wall -o lidarThreads lidarReadings.o runLidarThreads.c -lwiringPi -lm -lpthread