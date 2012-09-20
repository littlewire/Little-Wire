/*
	Updated for V1.1 firmware: August 2012
	Updated for C++: January 2012
	Created: December 2011
	by ihsan Kehribar <ihsan@kehribar.me>
	
	Tested with LTC1448:
		Dual 12-Bit Rail-to-Rail Micropower DAC
		
	Generates ramp signal on both output channels.
*/

#include <iostream>
#include "littleWire.h"

using namespace std;
unsigned char version;
unsigned int chA=0;
unsigned int chB=0;
unsigned char sendBuffer[4];
unsigned char receiveBuffer[4];	

int main()
{

	littleWire lw;
	
	if(lw.connect() == 0)
	{
		printf("Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}
	
	version = lw.readFirmwareVersion();
	printf("Little Wire firmware version: %d.%d\n",((version & 0xF0)>>4),(version&0x0F));	
	if(version==0x10)
	{
		printf("> This example requires the new 1.1 version firmware. Please update soon.\n");
		return 0;
	}
	
	lw.spi_init();
	
	// PIN3 will be used as a chip select pin
	lw.pinMode(PIN3,OUTPUT);
	lw.digitalWrite(PIN3,HIGH);
	
	lw.spi_updateDelay(0); // Change this according to your device. If your device doesn't respond, try to increase the delay
	
	for(;;){ // Generates triangular ramp signal
		if(chA<4096) chA++;
		else chA=0;
		if(chB<4096) chB++;
		else chB=0;
		
		sendBuffer[0]=(chA>>4);
		sendBuffer[1]=((chA&0x0F)<<4)+((chB&0xF00)>>8);
		sendBuffer[2]=(chB&0xFF);	
	
		lw.spi_sendMessage(sendBuffer,receiveBuffer,3,AUTO_CS); // Send 3 consequent messages with automatic chip select mode
		
		if(lw.isAlive()<0)
		{
			cout << "> lwStatus " << lw.isAlive() << endl;
			cout << "> Connection error!" << endl;
		}
	}
}
