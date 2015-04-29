all: lidarReadings.c lidarReadings.h runRobot.c redisFunctions.h redisFunctions.c
	gcc -c redisFunctions.c -I ~/hiredis -I ~/hiredis/adapters -lhiredis -O3 -fPIC -Wall -W -Wstrict-prototypes -Wwrite-strings -g
	gcc -c lidarReadings.c -lwiringPi -lm
	gcc -c driveCommand.c -I ~/hiredis
	gcc -c lidarProcessor.c -I ~/hiredis
	gcc -Wall -o runRobot ../wiringPiTest/driveControl.o lidarReadings.o driveCommand.o redisFunctions.o lidarProcessor.o runRobot.c -lwiringPi -lm -pthread -lhiredis -I ../../hiredis -L ~/hiredis

	
