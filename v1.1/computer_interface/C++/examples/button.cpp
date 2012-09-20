/*
	Updated for V1.1 firmware: August 2012
	Updated for C++: January 2012
	by ihsan Kehribar <ihsan@kehribar.me>
	Created: December 2011
	by Omer Kilic <omerkilic@gmail.com>
*/

#include <iostream>
#include "littleWire.h"

using namespace std;
unsigned char version;

#define BUTTON		PIN3	// Pin button is connected to PIN3 (active low)
#define DEBOUNCE	100		// Debounce delay, in miliseconds

int main()
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
	
	lw.pinMode(BUTTON, INPUT);
	lw.internalPullup(BUTTON,ENABLE);

	for(;;){
		if ( lw.digitalRead(BUTTON) == LOW ){
			delay(DEBOUNCE);
			if( lw.digitalRead(BUTTON) == LOW ){
				cout << "> Button pressed." << endl;
			}
		}
		
		if(lw.isAlive()<0)
		{
			cout << "> lwStatus " << lw.isAlive() << endl;
			cout << "> Connection error!" << endl;
		}
	}
	return 0;
}
