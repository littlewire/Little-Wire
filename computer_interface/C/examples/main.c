
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "littleWire.h"		/* Little Wire header file */

int main(int argc, char **argv)
{
	usb_dev_handle  *myLittleWire = NULL;

	myLittleWire = littleWire_connect();

	if(!myLittleWire)
	{
		getchar();
		return 0;
	}

	unsigned int adcValue;

	while(1)
	{
		adcValue=analogRead(myLittleWire,1);
		printf("Voltage: %f volts\n", (float)((adcValue*5.0)/1023.0));
	}
}

