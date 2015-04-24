lidarThreads: lidarReadings.c lidarReadings.h runLidarThreads.c redisFunctions.h redisFunctions.c
	gcc -c redisFunctions.c -I ~/hiredis -I ~/hiredis/adapters -lhiredis -O3 -fPIC -Wall -W -Wstrict-prototypes -Wwrite-strings -g
	gcc -c lidarReadings.c -lpthread -lwiringPi -lm
	gcc -Wall -o lidarThreads lidarReadings.o redisFunctions.o runLidarThreads.c -lwiringPi -lm -lpthread -lhiredis -I ../../hiredis -L ~/hiredis
	
