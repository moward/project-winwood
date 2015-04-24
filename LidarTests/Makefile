all: lidarReadings.c lidarReadings.h runRobot.c redisFunctions.h redisFunctions.c
	gcc -c redisFunctions.c -I ~/hiredis -I ~/hiredis/adapters -lhiredis -O3 -fPIC -Wall -W -Wstrict-prototypes -Wwrite-strings -g
	gcc -c lidarReadings.c -lwiringPi -lm
	gcc -c lidarProcessor.c
	gcc -Wall -o runRobot lidarReadings.o redisFunctions.o lidarProcessor.o runRobot.c -lwiringPi -lm -lpthread -lhiredis -I ../../hiredis -L ~/hiredis
	
