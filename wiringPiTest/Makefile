blink: blink.c
	gcc -Wall -o blink blink.c -lwiringPi
driveTest: driveControl.c pinAssignments.h driveTest.c
	gcc -c driveControl.c -lpthread
	gcc -Wall -o driveTest driveControl.o driveTest.c -lwiringPi -lm
clean:
	rm *.o driveTest blink