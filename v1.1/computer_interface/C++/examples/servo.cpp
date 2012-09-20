/*
	Updated for V1.1 firmware: August 2012
	Updated for C++: January 2012
	Created: December 2011
	by ihsan Kehribar <ihsan@kehribar.me>
*/

#include <iostream>
#include "littleWire.h"

using namespace std;
unsigned char version;

#define DELAY 10	// Delay, in miliseconds

int main(void)
{
	unsigned char currentLocation = 0;
	unsigned char direction=1;	
	littleWire lw;

	if(lw.connect() == 0)
	{
		printf("Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}
	
	version = lw.readFirmwareVersion();
	printf("Little Wire firmware version: %d.%d\n",((version & 0xF0)>>4),(version&0x0F));		
	if(version==0x10)
	{
		printf("> This example requires the new 1.1 version firmware. Please update soon.\n");
		return 0;
	}	
	
	lw.servo_init();

	for(;;)
	{
		cout << "Current locations: " << currentLocation << endl ;

		// Set two servo channels to the same location
		lw.servo_updateLocation(currentLocation,currentLocation);

		if(direction)
			currentLocation++;
		else
			currentLocation--;

		if(currentLocation==179)
			direction=0;
		else if(currentLocation==0)
			direction=1;

		delay(DELAY);
		
		if(lw.isAlive()<0)
		{
			cout << "> lwStatus " << lw.isAlive() << endl;
			cout << "> Connection error!" << endl;
		}
	}
}
