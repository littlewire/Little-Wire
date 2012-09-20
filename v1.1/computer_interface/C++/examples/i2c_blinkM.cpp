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
unsigned char i=0;
unsigned char rc=0;
unsigned char myBuffer[8];

int main()
{
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
	
	lw.i2c_init();
	lw.i2c_updateDelay(10);
	
	printf("> Address search ... \n");
	
	for(i=0;i<128;i++)
	{
		rc = lw.i2c_start(i,WRITE);	
		if(rc==1)
			printf("> Found device at %3d\n",i);
	}
	
	delay(2000);
	
	for(;;)
	{ 			
		/* Fade to a HSV color */
		lw.i2c_start(0x09,WRITE);						
			myBuffer[0] = 'h';
			myBuffer[1] = i++;
			myBuffer[2] = 255;
			myBuffer[3] = 255;		
		lw.i2c_write(myBuffer,4,END_WITH_STOP);
		
		delay(10);
		
		/* Get the current RGB color values */				
		lw.i2c_start(0x09,WRITE);		
			myBuffer[0] = 'g'; 
		lw.i2c_write(myBuffer,1,END_WITH_STOP);
		
		delay(10);
		
		/* Actually read the three bytes */
		lw.i2c_start(0x09,READ);
		lw.i2c_read(myBuffer,3,END_WITH_STOP);
		
		delay(10);		
		
		printf("> Read:\t#R: %2X\t#G: %2X\t#B: %2X\t\n",myBuffer[0],myBuffer[1],myBuffer[2]);				
		
		if(lw.isAlive()<0)
		{
			printf("> lwStatus: %d\n",lw.isAlive());
			printf("> Connection error!\n");
			return 0;
		}
	}
	return 0;
}
