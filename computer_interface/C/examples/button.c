/*	
	Created: December 2011
	by Omer Kilic <omerkilic@gmail.com>
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "littleWire.h"

#if defined(LINUX)
	#define	sleep_ms(duration) sleep(duration)
#else
	#define	sleep_ms(duration) Sleep(duration)
#endif

#define BUTTON		MISO_PIN	// Pin button is connected to (active low)
#define DEBOUNCE	10			// Debounce delay, in microseconds

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
			sleep_ms(DEBOUNCE);
			if( digitalRead(myLittleWire, BUTTON) == LOW ){
				printf("Button pressed.\n");
			}
		}
		sleep_ms(1);
	}
}
