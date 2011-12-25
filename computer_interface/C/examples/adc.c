#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "littleWire.h"

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
		adcValue=analogRead(myLittleWire, ADC0);
		printf("Voltage: %f volts\n", (float)((adcValue*5.0)/1024.0));
		sleep(1);
	}
}
