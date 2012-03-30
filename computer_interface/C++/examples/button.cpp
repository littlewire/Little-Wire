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

#define BUTTON		PIN3	// Pin button is connected to PIN3 (active low)
#define DEBOUNCE	100		// Debounce delay, in miliseconds

int main()
{
	littleWire myLittleWire;

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
