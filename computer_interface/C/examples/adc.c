/*
	Created: December 2011
	by ihsan Kehribar <ihsan@kehribar.me>
*/

#include <stdio.h>
#include <stdlib.h>
#include "littleWire.h"
#include "littleWire_util.h"


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
		printf("Value: %d\n",adcValue);
		//printf("Voltage: %f volts\n", (float)((adcValue*5.0)/1024.0));
		//delay(1000);
	}
}
