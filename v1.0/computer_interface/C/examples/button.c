/*
	Created: December 2011
	by Omer Kilic <omerkilic@gmail.com>
*/

#include <stdio.h>
#include <stdlib.h>
#include "littleWire.h"
#include "littleWire_util.h"

#define BUTTON		PIN3	// Pin button is connected to pin3 (active low)
#define DEBOUNCE	100		// Debounce delay, in miliseconds

unsigned char version;

int main()
{
	littleWire *myLittleWire = NULL;

	myLittleWire = littleWire_connect();

	if(myLittleWire == NULL){
		printf("Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}
	
	version = readFirmwareVersion(myLittleWire);
	printf("Little Wire firmware version: %d.%d\n",((version & 0xF0)>>4),(version&0x0F));	

	pinMode(myLittleWire, BUTTON, INPUT);

	for(;;){
		if ( digitalRead(myLittleWire, BUTTON) == LOW ){
			delay(DEBOUNCE);
			if( digitalRead(myLittleWire, BUTTON) == LOW ){
				printf("Button pressed.\n");
			}
		}
		delay(100);
	}
}
