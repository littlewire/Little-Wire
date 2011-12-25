
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "littleWire.h"  /* Little Wire header file */

#define LED   2
#define DELAY 1*1000*1000 /* In microseconds */

int main(int argc, char **argv)
{
	usb_dev_handle *myLittleWire = NULL;

	myLittleWire = littleWire_connect();

	if(myLittleWire == NULL){
		printf("Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}

	pinMode(myLittleWire, LED, 0);

	while(1)
	{
		digitalWrite(myLittleWire, LED, 1);
		usleep(DELAY);
		digitalWrite(myLittleWire, LED, 0);
		usleep(DELAY);
	}
}

