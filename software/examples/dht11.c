/*
	Updated for the new firmware: July 2012
	by ihsan Kehribar <ihsan@kehribar.me>
	
	Created: December 2011
	by ihsan Kehribar <ihsan@kehribar.me>
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "littleWire.h"
#include "littleWire_util.h"

unsigned char version;

dht_reading val;

int main(int argc, char **argv)
{
	littleWire *lw = NULL;

	lw = littleWire_connect();

	if(lw == NULL){
		printf("> Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}

	version = readFirmwareVersion(lw);
	printf("> Little Wire firmware version: %d.%d\n",((version & 0xF0)>>4),(version&0x0F));	
	if(version<0x14)
	{
		printf("> This example requires the new 1.4 version firmware. Please update soon.\n");
		return 0;
	}
		
	while(1)
	{
    val = dht_read(lw, DHT11);
    if (!val.error) {
    printf("humidity: %f, temp %f\n", (float)val.humid/10.0, (float)val.temp/10.0);
    } else {
      printf("Error Reading sensor!");
    }
		
		if(lwStatus<0)
		{
			printf("> lwStatus: %d\n",lwStatus);
			printf("> Connection error!\n");
			return 0;
		}
		
		delay(5000);
	}
}
