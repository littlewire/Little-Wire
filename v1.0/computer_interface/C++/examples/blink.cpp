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

#define LED		PIN4	// Pin LED is connected to PIN4
#define DELAY	500		// Delay, in miliseconds

int main(void)
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
	
	myLittleWire.pinMode(LED, OUTPUT);

	for(;;){
		cout << "Blink!" << endl;
		myLittleWire.digitalWrite(LED, HIGH);
		delay(DELAY);
		myLittleWire.digitalWrite(LED, LOW);
		delay(DELAY);
	}
	return 0;
}
