blink: blink.c
	gcc -Wall -o blink blink.c -lwiringPi
driveTest: driveControl.c pinAssignments.h driveTest.c
	gcc -c driveControl.c -pthread -L /usr/lib
	gcc -Wall -o driveTest driveControl.o driveTest.c -lwiringPi -lm -pthread
clean:
	rm *.o driveTest blink
