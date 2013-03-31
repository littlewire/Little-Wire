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
unsigned char version;

int main()
{
	unsigned int adcValue;	
	littleWire myLittleWire;
	
	if(myLittleWire.connect() == 0)
	{
		printf("Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}
	
	version = myLittleWire.readFirmwareVersion();
	printf("Little Wire firmware version: %d.%d\n",((version & 0xF0)>>4),(version&0x0F));	
	
	myLittleWire.pwm_stop();

	for(;;){
		adcValue=myLittleWire.analogRead(ADC_PIN2);		
		cout << "Voltage: " <<(float)((adcValue*5.0)/1024.0) << " volts" << endl;
		delay(100);
	}
	
	return 0;
}
