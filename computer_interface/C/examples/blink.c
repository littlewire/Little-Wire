/*
	Created: December 2011
	by Omer Kilic <omerkilic@gmail.com>
*/

#include <stdio.h>
#include <stdlib.h>
#include "littleWire.h"
#include "littleWire_util.h"

#define LED		PIN4		// LED is connected to the pin4
#define DELAY	500 		// Delay, in miliseconds

unsigned char version;

int main(void)
{
	littleWire *myLittleWire = NULL;

	myLittleWire = littleWire_connect();

	if(myLittleWire == NULL){
		printf("Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}
	
	version = readFirmwareVersion(myLittleWire);
	printf("Little Wire firmware version: %d.%d\n",((version & 0xF0)>>4),(version&0x0F));

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
