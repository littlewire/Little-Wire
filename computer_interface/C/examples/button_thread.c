/*	
	-- This is experimental!! --
	Created: December 2011
	by Omer Kilic <omerkilic@gmail.com>
*/

#ifndef LINUX
  #warning "This probably won't work outside Linux."
#endif

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "littleWire.h"

#if defined(LINUX)
	#define	sleep_ms(duration) sleep(duration)
#else
	#define	sleep_ms(duration) Sleep(duration)
#endif

#define BUTTON		MISO_PIN	// Pin button is connected to (active low)
#define DEBOUNCE	100			// Debounce time, in miliseconds

littleWire *myLittleWire = NULL;

void *buttonHandler(void *arg)
{
	int buttonPin = (int)arg;

	for(;;){
		if ( digitalRead(myLittleWire, buttonPin) == LOW ){
			sleep_ms(DEBOUNCE);
			if( digitalRead(myLittleWire, buttonPin) == LOW ){
				printf("\nButton pressed.\n");
			}
		}
		sleep_ms(1);
	}
}

int main()
{
	pthread_t buttonThread;

	myLittleWire = littleWire_connect();

	if(myLittleWire == NULL){
		printf("Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}

	pinMode(myLittleWire, BUTTON, INPUT);

	pthread_create(&buttonThread, NULL, buttonHandler, (void*)BUTTON);
	pthread_tryjoin_np(buttonThread, BUTTON);

	for(;;){
		// this is where main() does stuff
	}

}
