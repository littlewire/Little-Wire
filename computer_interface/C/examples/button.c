/*
	Created: December 2011
	by Omer Kilic <omerkilic@gmail.com>
*/

#include <stdio.h>
#include <stdlib.h>
#include "littleWire.h"
#include "littleWire_util.h"

#define BUTTON		PIN1	// Pin button is connected to pin1 (active low)
#define DEBOUNCE	100		// Debounce delay, in miliseconds

int main()
{
	littleWire *myLittleWire = NULL;

	myLittleWire = littleWire_connect();

	if(myLittleWire == NULL){
		printf("Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}

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
