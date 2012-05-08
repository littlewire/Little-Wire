/*
	Updated for C++: January 2012
	Created: December 2011
	by ihsan Kehribar <ihsan@kehribar.me>
	
	Tested with LTC1448:
		Dual 12-Bit Rail-to-Rail Micropower DAC

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
	unsigned int chA=0;
	unsigned int chB=0;
	littleWire myLittleWire;
	
	if(myLittleWire.connect() == 0)
	{
		printf("Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}
	
	version = myLittleWire.readFirmwareVersion();
	printf("Little Wire firmware version: %d.%d\n",((version & 0xF0)>>4),(version&0x0F));	
	
	myLittleWire.pwm_stop();
	
	myLittleWire.spi_init();
	
	// PIN3 will be used as a chip select pin
	myLittleWire.pinMode(PIN3,OUTPUT);
	myLittleWire.digitalWrite(PIN3,HIGH);
	
	myLittleWire.spi_updateDelay(0); // Change this according to your device. If your device doesn't respond, try to increase the delay

	unsigned char sendBuffer[4];
	unsigned char receiveBuffer[4];
	
	for(;;){ // Generates triangular ramp signal
		if(chA<4096) chA++;
		else chA=0;
		if(chB<4096) chB++;
		else chB=0;
		
		sendBuffer[0]=(chA>>4);
		sendBuffer[1]=((chA&0x0F)<<4)+((chB&0xF00)>>8);
		sendBuffer[2]=(chB&0xFF);	
	
		myLittleWire.spi_sendMessageMulti(sendBuffer,receiveBuffer,3,AUTO_CS); // Send 3 consequent messages with automatic chip select mode
	
		// Alternative with manual chip select 
		/*
			myLittleWire.digitalWrite(PIN3,LOW); // Chip select low		
				myLittleWire.spi_sendMessageMulti(sendBuffer,receiveBuffer,3,MANUAL_CS);
			myLittleWire.digitalWrite(PIN3,HIGH); // Chip select high
		*/
	}
}
