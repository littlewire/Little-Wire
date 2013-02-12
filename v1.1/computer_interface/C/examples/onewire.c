/*
	Created: July 2012
	by ihsan Kehribar <ihsan@kehribar.me>
	
	Example program to search onewire bus and read temperature from
	DS1820 type digital sensors.
	
	Data pin: PIN2
*/

#include <stdio.h>
#include <stdlib.h>
#include "littleWire.h"
#include "littleWire_util.h"

littleWire *lw = NULL;

unsigned char version;
unsigned char rxx[8];
int rc=0;
int i=0;
int t=0;

int main(void)
{
        unsigned char temphigh;
        unsigned char templow;
        unsigned char tempdecimal;
        unsigned char scratch;

	lw = littleWire_connect();

	if(lw == NULL){
		printf("Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}
	
	version = readFirmwareVersion(lw);
	printf("> Little Wire firmware version: %d.%d\n",((version & 0xF0)>>4),(version&0x0F));
	if(version==0x10)
	{
		printf("This example requires the new 1.1 version firmware. Please update soon.\n");
		return 0;
	}
	
	printf("> Address search ... \n");
	
	if(onewire_firstAddress(lw))
	{
		printf("> Addr#%d:  %X\t",i,ROM_NO[0]);
		for(t=0;t<7;t++)
			printf("%X\t",ROM_NO[t+1]);			
		printf("\n");
		i=1;
	}

	while(onewire_nextAddress(lw))
	{
		printf("> Addr#%d:  %X\t",i,ROM_NO[0]);
		for(t=0;t<7;t++)
			printf("%X\t",ROM_NO[t+1]);			
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
		if(!onewire_resetPulse(lw)) // if this returns zero, sensor is unplugged		
		{
			printf("> Sensor is unplugged!\n");
			return 0;
		}
		
		// onewire_writeByte(0xCC); /* SKIP ROM command */
		onewire_writeByte(lw,0x55); /* MATCH ROM command */		
		
		onewire_writeByte(lw,ROM_NO[0]); /* Address begins */
		onewire_writeByte(lw,ROM_NO[1]);
		onewire_writeByte(lw,ROM_NO[2]);
		onewire_writeByte(lw,ROM_NO[3]);
		onewire_writeByte(lw,ROM_NO[4]);
		onewire_writeByte(lw,ROM_NO[5]);
		onewire_writeByte(lw,ROM_NO[6]);
		onewire_writeByte(lw,ROM_NO[7]); /* Address ends */	
		
		onewire_writeByte(lw,0x44); /* CONVERT! command */		

		// wait for conversion ?
		delay(450);
		
		// send reset signal and read the presence value
		if(!onewire_resetPulse(lw)) // if this returns zero, sensor is unplugged		
		{
			printf("> Sensor is unplugged!\n");
			return 0;
		}
			
		// onewire_writeByte(0xCC); /* SKIP ROM command */
		onewire_writeByte(lw,0x55); /* MATCH ROM command */		
		
		onewire_writeByte(lw,ROM_NO[0]); /* Address begins */
		onewire_writeByte(lw,ROM_NO[1]);
		onewire_writeByte(lw,ROM_NO[2]);
		onewire_writeByte(lw,ROM_NO[3]);
		onewire_writeByte(lw,ROM_NO[4]);
		onewire_writeByte(lw,ROM_NO[5]);
		onewire_writeByte(lw,ROM_NO[6]);
		onewire_writeByte(lw,ROM_NO[7]); /* Address ends */	
		
		onewire_writeByte(lw,0xBE); /* Read register command */
		
		printf("> ");
		
		for(i=0;i<9;i++)
		{	
                        scratch = onewire_readByte(lw);
                        printf("%2X\t",scratch);
                        if (i == 0)
                                templow = scratch;
                        if (i == 1)
                                temphigh = scratch;
		}

		switch (templow & 0x0c) {
                        case 0x00:
                                tempdecimal = 0;
                                break;
                        case 0x04:
                                tempdecimal = 25;
                                break;
                        case 0x08:
                                tempdecimal = 5;
                                break;
                        case 0x0c:
                                tempdecimal = 75;
                                break;
                        default:
                                tempdecimal = 0;
                }

                printf("> TEMP %c%d.%d", ((temphigh & 0x80) == 0x80) ? '-' : ' ', ((templow & 0xf0) >> 4) | ((temphigh & 0x07) << 4), tempdecimal);
                printf("\n");
		
		if(lwStatus<0)
		{
			printf("> lwStatus: %d\n",lwStatus);
			printf("> Connection error!\n");
			return 0;
		}
		
	}
	
}
