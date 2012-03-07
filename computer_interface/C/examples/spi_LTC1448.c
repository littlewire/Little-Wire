/*
	Created: December 2011
	by ihsan Kehribar <ihsan@kehribar.me>
	
	Tested with LTC1448:
		Dual 12-Bit Rail-to-Rail Micropower DAC

*/

#include <stdio.h>
#include <stdlib.h>
#include "littleWire.h"
#include "littleWire_util.h"

int main()
{
	unsigned int chA=0;
	unsigned int chB=0;

	littleWire *myLittleWire = NULL;

	myLittleWire = littleWire_connect();

	if(myLittleWire == NULL){
		printf("Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}

	spi_init(myLittleWire);
	
	// pin3 will be used as our chip select pin
	pinMode(myLittleWire,PIN3,OUTPUT);
	digitalWrite(myLittleWire,PIN3,HIGH);
	
	spi_updateDelay(myLittleWire,0); // Change this according to your device. If your device doesn't respond, try to increase the delay

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
	
		spi_sendMessage_multiple(myLittleWire,sendBuffer,receiveBuffer,3,AUTO_CS); // Send 3 consequent messages with automatic chip select mode
		delay(2000);
		// Alternative with manual chip select 
		/*
			digitalWrite(myLittleWire,PIN3,LOW); // Chip select low		
				sendSpiMessage_multiple(myLittleWire,sendBuffer,receiveBuffer,3,MANUAL_CS);
			digitalWrite(myLittleWire,PIN3,HIGH); // Chip select high
		*/
	}
}
