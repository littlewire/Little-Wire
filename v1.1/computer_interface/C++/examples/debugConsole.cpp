/*
	Updated for C++: August 2012
	Created July 2012
	by ihsan Kehribar <ihsan@kehribar.me>
*/

#include <stdio.h>
#include <stdlib.h>
#include "littleWire.h"
#include "littleWire_util.h"

unsigned char version;
unsigned int adcValue;

int main(int argc, char **argv)
{
	littleWire lw;
	
	if(lw.connect() == 0)
	{
		printf("> Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}
	
	version = lw.readFirmwareVersion();
	printf("> Little Wire firmware version: %d.%d\n",((version & 0xF0)>>4),(version&0x0F));	
	if(version==0x10)
	{
		printf("> This example requires the new 1.1 version firmware. Please update soon.\n");
		return 0;
	}	

	char input;

	while(1)
	{
		input=lw.debugSpi(0x00);
		if(input!=0)
			printf("%c",input);
			
		if(lw.isAlive()<0)
		{
			printf("> lwStatus: %d\n",lw.isAlive());
			printf("> Connection error!\n");
			return 0;
		}
	}
}
