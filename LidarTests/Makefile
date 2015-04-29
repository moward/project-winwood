all: lidarReadings.c lidarReadings.h runRobot.c redisFunctions.h redisFunctions.c
	gcc -c redisFunctions.c -I ~/hiredis -I ~/hiredis/adapters -lhiredis -O3 -fPIC -Wall -W -Wstrict-prototypes -Wwrite-strings -g
	gcc -c lidarReadings.c -lwiringPi -lm
	gcc -c lidarProcessor.c -lm
	gcc -Wall -o runRobot lidarReadings.o redisFunctions.o lidarProcessor.o runRobot.c -lwiringPi -lm -lpthread -lhiredis -I ../../hiredis -L ~/hiredis
	
processorTest: lidarProcessor.h lidarProcessor.c lidarProcessorTest.c
	gcc -c lidarProcessor.c -lm
	gcc -c redisFunctionsStub.c
	gcc -Wall -o lidarProcessorTest redisFunctionsStub.c lidarProcessor.o lidarProcessorTest.c -lm
