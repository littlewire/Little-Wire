/*
	Created: December 2011
	by ihsan Kehribar <ihsan@kehribar.me>
*/

#include <stdio.h>
#include <stdlib.h>
#include "littleWire.h"
#include "littleWire_util.h"

unsigned char version;

int main(int argc, char **argv)
{
	littleWire *myLittleWire = NULL;
	unsigned int adcValue;

	myLittleWire = littleWire_connect();

	if(myLittleWire == NULL){
		printf("Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}
	
	version = readFirmwareVersion(myLittleWire);
	printf("Little Wire firmware version: %d.%d\n",((version & 0xF0)>>4),(version&0x0F));
	
	pinMode(myLittleWire,PIN2,INPUT);

	while(1){
		adcValue=analogRead(myLittleWire, ADC_PIN2);
		printf("Value: %d\tVoltage: %f volts\n", adcValue,(float)((adcValue*5.0)/1024.0));
		delay(100);
	}
}
