/*
	Created: December 2011
	by ihsan Kehribar <ihsan@kehribar.me>
*/

#include <stdio.h>
#include <stdlib.h>
#include "littleWire.h"
#include "littleWire_util.h"
#include "littleWire_servo.h"

#define DELAY 10	// Delay, in miliseconds

unsigned char currentLocation = 0;
unsigned char direction=1;	

int main(void)
{
	littleWire *myLittleWire = NULL;
	
	myLittleWire = littleWire_connect();

	if(myLittleWire == NULL){
		printf("Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}

	servo_init(myLittleWire);

	for(;;)
	{
		printf("Current locations: %d\n",currentLocation);

		// Set two servo channels to the same location
		servo_updateLocation(myLittleWire,currentLocation,currentLocation);

		if(direction)
			currentLocation++;
		else
			currentLocation--;

		if(currentLocation==180)
			direction=0;
		else if(currentLocation==0)
			direction=1;

		delay(DELAY);
	}
}
