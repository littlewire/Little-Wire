/*
	Created: April 2012
	by ihsan Kehribar <ihsan@kehribar.me>
	
	Basic terminal application for printf style debugging over AVR-ISP pins.
	
*/

#include <stdio.h>
#include <stdlib.h>
#include "littleWire.h"
#include "littleWire_util.h"

unsigned char temp;
int enable=0;

int main()
{
	littleWire *myLittleWire = NULL;

	myLittleWire = littleWire_connect();

	if(myLittleWire == NULL){
		printf("Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}					
	
	printf("Press Ctrl+c to exit\n");
	
	delay(1500);	
	
	for(;;)
	{	
		temp=debug_rw(myLittleWire,0,2);
		if(temp>0)
			printf("%C",temp);
		delay(2);
	}
	return 0;
}
