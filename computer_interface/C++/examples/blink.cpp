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

#define LED		PIN3	// Pin LED is connected to MOSI (PIN0) pin
#define DELAY	500		// Delay, in miliseconds

int main(void)
{
	littleWire myLittleWire;

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
