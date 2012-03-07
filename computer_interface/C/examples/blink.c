/*
	Created: December 2011
	by Omer Kilic <omerkilic@gmail.com>
*/

#include <stdio.h>
#include <stdlib.h>
#include "littleWire.h"
#include "littleWire_util.h"

#define LED		PIN1		// LED is connected to the pin1
#define DELAY	500 		// Delay, in miliseconds

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
		delay(DELAY);
		printf("...\n");
		digitalWrite(myLittleWire, LED, LOW);
		delay(DELAY);
	}
	
}
