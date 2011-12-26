/*
	Created: December 2011
	by Omer Kilic <omerkilic@gmail.com>
*/

#include <stdio.h>
#include <stdlib.h>
#include "littleWire.h"
#include "littleWire_util.h"


#define LED	PIN0		// Pin LED is connected to MOSI (PIN0) pin
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
		digitalWrite(myLittleWire, LED, LOW);
		delay(DELAY);
	}
}
