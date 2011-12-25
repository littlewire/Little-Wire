#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "littleWire.h"

#define LED	PIN3	// Pin LED is connected to
#define DELAY	1	// Delay, in seconds

int main(void)
{
	littleWire *myLittleWire = NULL;

	myLittleWire = littleWire_connect();

	if(myLittleWire == NULL){
		printf("Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}

	pinMode(myLittleWire, LED, OUTPUT);

	for(;;){
		printf("Blink!\n");
		digitalWrite(myLittleWire, LED, HIGH);
		sleep(DELAY);
		digitalWrite(myLittleWire, LED, LOW);
		sleep(DELAY);
	}
}
