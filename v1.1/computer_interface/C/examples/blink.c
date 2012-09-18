/*
	Updated for the new firmware: July 2012
	by ihsan Kehribar <ihsan@kehribar.me>

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
	littleWire *lw = NULL;

	lw = littleWire_connect();

	if(lw == NULL){
		printf("> Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}
	
	version = readFirmwareVersion(lw);
	printf("> Little Wire firmware version: %d.%d\n",((version & 0xF0)>>4),(version&0x0F));	
	if(version==0x10)
	{
		printf("> This example requires the new 1.1 version firmware. Please update soon.\n");
		return 0;
	}	
	
	pinMode(lw, LED, OUTPUT);

	for(;;){
		printf("Blink!\n");
		digitalWrite(lw, LED, HIGH);
		delay(DELAY);
		printf("...\n");
		digitalWrite(lw, LED, LOW);
		delay(DELAY);
	
		if(lwStatus<0)
		{
			printf("> lwStatus: %d\n",lwStatus);
			printf("> Connection error!\n");
			return 0;
		}
	}
	
}