/*
	Updated for C++: August 2012
	Created: July 2012
	by ihsan Kehribar <ihsan@kehribar.me>
	
	Example program to search onewire bus and read temperature from
	DS1820 type digital sensors.
	
	Data pin: PIN2
*/

#include <stdio.h>
#include <stdlib.h>
#include "littleWire.h"

unsigned char version;
unsigned char rxx[8];
unsigned char myData[8];
int rc=0;
int i=0;
int t=0;

int main(void)
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
	
	printf("> Address search ... \n");
	
	if(lw.onewire_firstAddress(myData))
	{
		printf("> Addr #%d:  %X\t",i,myData[0]);
		for(t=0;t<7;t++)
			printf("%X\t",myData[t+1]);			
		printf("\n");
		i=1;
	}

	while(lw.onewire_nextAddress(myData))
	{
		printf("> Addr #%d:  %X\t",i,myData[0]);
		for(t=0;t<7;t++)
			printf("%X\t",myData[t+1]);			
		printf("\n");
		i++;
	}

	if(i>0)
		printf("> End of search with %d device(s) found\r\n",i);	
	else
	{
		printf("> No device has been found!\n");
		return 0;
	}
	
	printf("> Start the talking with the last device found\n");
	
	delay(2000);
	
	for(;;)
	{			
		// send reset signal and read the presence value
		if(!lw.onewire_resetPulse()) // if this returns zero, sensor is unplugged		
		{
			printf("> Sensor is unplugged!\n");
			return 0;
		}
		
		// lw.onewire_writeByte(0xCC); /* SKIP ROM command */
		lw.onewire_writeByte(0x55); /* MATCH ROM command */		
		
		lw.onewire_writeByte(myData[0]); /* Address begins */
		lw.onewire_writeByte(myData[1]);
		lw.onewire_writeByte(myData[2]);
		lw.onewire_writeByte(myData[3]);
		lw.onewire_writeByte(myData[4]);
		lw.onewire_writeByte(myData[5]);
		lw.onewire_writeByte(myData[6]);
		lw.onewire_writeByte(myData[7]); /* Address ends */	
		
		lw.onewire_writeByte(0x44); /* CONVERT! command */		

		// wait for conversion ?
		delay(450);
		
		// send reset signal and read the presence value
		if(!lw.onewire_resetPulse()) // if this returns zero, sensor is unplugged		
		{
			printf("> Sensor is unplugged!\n");
			return 0;
		}
			
		// lw.onewire_writeByte(0xCC); /* SKIP ROM command */
		lw.onewire_writeByte(0x55); /* MATCH ROM command */		
		
		lw.onewire_writeByte(myData[0]); /* Address begins */
		lw.onewire_writeByte(myData[1]);
		lw.onewire_writeByte(myData[2]);
		lw.onewire_writeByte(myData[3]);
		lw.onewire_writeByte(myData[4]);
		lw.onewire_writeByte(myData[5]);
		lw.onewire_writeByte(myData[6]);
		lw.onewire_writeByte(myData[7]); /* Address ends */	
		
		lw.onewire_writeByte(0xBE); /* Read register command */
		
		printf("> ");
		
		for(i=0;i<9;i++)
		{	
			printf("%2X\t",lw.onewire_readByte());				
		}
		printf("\n");
		
		if(lw.isAlive()<0)
		{
			printf("> lwStatus: %d\n",lw.isAlive());
			printf("> Connection error!\n");
			return 0;
		}
		
	}
	
}
