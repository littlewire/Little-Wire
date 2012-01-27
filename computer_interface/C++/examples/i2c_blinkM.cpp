/*
	Updated for C++: January 2012
	Created: December 2011
	by ihsan Kehribar <ihsan@kehribar.me>
	
	Tested with 10Kohm pullup resistors on SDA and SCL lines.
	
*/

#include <iostream>
#include "littleWire.h"
extern "C" 
{ 
	#include "littleWire_util.h" 
}

using namespace std;

int main()
{
	unsigned char i=0;
	littleWire myLittleWire;

	myLittleWire.i2c_init();
	
	for(;;){ // Constantly change colors ...
		myLittleWire.i2c_beginTransmission(0x09);
		myLittleWire.i2c_send('c');
		myLittleWire.i2c_send(i++);
		myLittleWire.i2c_send(i*3);
		myLittleWire.i2c_send(255-i);
		myLittleWire.i2c_endTransmission();
	}
	return 0;
}
