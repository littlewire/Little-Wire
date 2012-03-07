/*
	Created: December 2011
	by ihsan Kehribar <ihsan@kehribar.me>
	
	Tested with 10Kohm pullup resistors on SDA and SCL lines.	
*/

#include <stdio.h>
#include <stdlib.h>
#include "littleWire.h"
#include "littleWire_util.h"

unsigned char i=0;

int main()
{
	littleWire *myLittleWire = NULL;

	myLittleWire = littleWire_connect();

	if(myLittleWire == NULL){
		printf("Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}

	i2c_init(myLittleWire);
	
	for(;;){ // Constantly change colors ...
		i2c_beginTransmission(myLittleWire,0x09);
		i2c_send(myLittleWire,'c');
		i2c_send(myLittleWire,i++);
		i2c_send(myLittleWire,i*3);
		i2c_send(myLittleWire,255-i);
		i2c_endTransmission(myLittleWire);
		delay(100);
	}
}
