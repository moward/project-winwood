lidarThreads: lidarReadings.c lidarReadings.h runLidarThreads.c
	gcc -c lidarReadings.c -lpthread -lwiringPi -lm
	gcc -Wall -o lidarThreads lidarReadings.o runLidarThreads.c -lwiringPi -lm -lpthread -I ../../hiredis

redisFunctions: redisFunctions.c redisFunctions.h
	gcc -c redisFunctions.c -I ~/hiredis ~/hiredis/adapters -lhiredis