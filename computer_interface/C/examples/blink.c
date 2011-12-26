/*	
	Created: December 2011
	by Omer Kilic <omerkilic@gmail.com>
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "littleWire.h"

#if defined(LINUX)
	#define	sleep_ms(duration) sleep(duration)
#else
	#define	sleep_ms(duration) Sleep(duration)
#endif

#define LED	MOSI_PIN	// Pin LED is connected to MOSI pin
#define DELAY 500 		// Delay, in miliseconds

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
		sleep_ms(DELAY);
		digitalWrite(myLittleWire, LED, LOW);
		sleep_ms(DELAY);
	}
}
