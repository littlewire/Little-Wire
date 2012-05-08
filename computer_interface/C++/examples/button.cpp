/*
	Updated for C++: January 2012
	Created: December 2011
	by Omer Kilic <omerkilic@gmail.com>
*/

#include <iostream>
#include "littleWire.h"
extern "C" 
{ 
	#include "littleWire_util.h" 
}

using namespace std;
unsigned char version;

#define BUTTON		PIN3	// Pin button is connected to PIN3 (active low)
#define DEBOUNCE	100		// Debounce delay, in miliseconds

int main()
{
	littleWire myLittleWire;

	if(myLittleWire.connect() == 0)
	{
		printf("Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}
	
	version = myLittleWire.readFirmwareVersion();
	printf("Little Wire firmware version: %d.%d\n",((version & 0xF0)>>4),(version&0x0F));		
	
	myLittleWire.pwm_stop();
	
	myLittleWire.pinMode(BUTTON, INPUT);

	for(;;){
		if ( myLittleWire.digitalRead(BUTTON) == LOW ){
			delay(DEBOUNCE);
			if( myLittleWire.digitalRead(BUTTON) == LOW ){
				cout << "Button pressed." << endl;
			}
		}
	}
	return 0;
}
