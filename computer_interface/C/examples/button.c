#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "littleWire.h"

#define BUTTON		PIN1		// Pin button is connected to (active low)
#define DEBOUNCE	100*1000	// Debounce delay, in microseconds

int main()
{
	littleWire *myLittleWire = NULL;

	myLittleWire = littleWire_connect();

	if(myLittleWire == NULL){
		printf("Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}

	pinMode(myLittleWire, BUTTON, 1);

	for(;;){
		if ( digitalRead(myLittleWire, BUTTON) == 0 ){
			usleep(DEBOUNCE);
			if( digitalRead(myLittleWire, BUTTON) == LOW ){
				printf("Button pressed.\n");
			}
		}

		sleep(1);
	}
}
