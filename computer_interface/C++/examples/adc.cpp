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

int main()
{
	unsigned int adcValue;	
	littleWire myLittleWire;

	for(;;){
		adcValue=myLittleWire.analogRead(ADC_SCK_PIN);		
		cout << "Voltage: " <<(float)((adcValue*5.0)/1024.0) << " volts" << endl;
		delay(1000);
	}
	
	return 0;
}
