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
int main()
{
	littleWire *myLittleWire = NULL;

	myLittleWire = littleWire_connect();

	if(myLittleWire == NULL){
		printf("Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}					

	printf("Press Ctrl+c to exit\n");
	delay(3000);
	
	for(;;){			
		temp=debug_rw(myLittleWire,0,1);
		if(temp>0)
			printf("%C",temp);
	}
}
