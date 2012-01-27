/*
	Updated for C++: January 2012
	Created: December 2011
	by ihsan Kehribar <ihsan@kehribar.me>
*/

#include <iostream>
#include "littleWire.h"
extern "C" 
{ 
	#include "littleWire_util.h" 
}

using namespace std;

#define DELAY 10	// Delay, in miliseconds

int main(void)
{
	unsigned char currentLocation = 0;
	unsigned char direction=1;	
	littleWire myLittleWire;

	myLittleWire.servo_init();

	for(;;)
	{
		cout << "Current locations: " << currentLocation << endl ;

		// Set two servo channels to the same location
		myLittleWire.servo_updateLocation(currentLocation,currentLocation);

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
