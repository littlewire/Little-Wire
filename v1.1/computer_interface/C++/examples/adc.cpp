/*
	Updated for V1.1 firmware: August 2012
	Updated for C++: January 2012
	Created: December 2011
	by ihsan Kehribar <ihsan@kehribar.me>
*/

#include <iostream>
#include "littleWire.h"

using namespace std;
unsigned char version;

int main()
{
	unsigned int adcValue;	
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
	
	// -- Select ADC reference voltage
	// Those definetly needs calibration for precision measurement. 
	lw.analog_init(VREF_VCC);
	// lw.analog_init(VREF_2560mV);
	// lw.analog_init(VREF_1100mV);
	
	for(;;){
		adcValue=lw.analogRead(ADC_PIN2);		
		cout << "Voltage: " <<(float)((adcValue*5.0)/1024.0) << " volts" << endl;
		// cout << "Voltage: " <<(float)((adcValue*2.56)/1024.0) << " volts" << endl;
		// cout << "Voltage: " <<(float)((adcValue*1.1)/1024.0) << " volts" << endl;
		delay(100);
		
		if(lw.isAlive()<0)
		{
			cout << "> lwStatus " << lw.isAlive() << endl;
			cout << "> Connection error!" << endl;
		}
	}
	
	return 0;
}
