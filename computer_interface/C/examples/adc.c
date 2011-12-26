/*	
	Created: December 2011
	by ihsan Kehribar <ihsan@kehribar.me>
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

int main(int argc, char **argv)
{
	littleWire *myLittleWire = NULL;
	unsigned int adcValue;

	myLittleWire = littleWire_connect();

	if(myLittleWire == NULL){
		printf("Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}

	while(1){
		adcValue=analogRead(myLittleWire, ADC_SCK_PIN);
		printf("Voltage: %f volts\n", (float)((adcValue*5.0)/1024.0));
		sleep_ms(1);
	}
}
