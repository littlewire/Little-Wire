/*
	Created: December 2011
	by Omer Kilic <omerkilic@gmail.com>
*/

#include <stdio.h>
#include <stdlib.h>
#include "littleWire.h"
#include "littleWire_util.h"


#define BUTTON		PIN1	// Pin button is connected to (active low)
#define DEBOUNCE	100	// Debounce delay, in microseconds


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
		if ( digitalRead(myLittleWire, BUTTON) == 0 ){
			delay(DEBOUNCE);
			if( digitalRead(myLittleWire, BUTTON) == LOW ){
				printf("Button pressed.\n");
			}
		}
		delay(1000);
	}
}
