/*
	Created: December 2011
	by ihsan Kehribar <ihsan@kehribar.me>
	
	Tested with 10Kohm pullup resistors on SDA and SCL lines.
	
*/

#include <stdio.h>
#include <stdlib.h>
#include "littleWire.h"
#include "littleWire_util.h"

unsigned char receiveBuffer[8];
int index=0;
int addressFound=0;
int t=0;

int main()
{
	littleWire *myLittleWire = NULL;

	myLittleWire = littleWire_connect();

	if(myLittleWire == NULL){
		printf("Little Wire could not be found!\n");
		exit(EXIT_FAILURE);
	}

	i2c_init(myLittleWire);
	
	// Look for the address
	/*for(t=0;t<256;t++)
	{
		i2c_beginTransmission(myLittleWire,t);
		i2c_send(myLittleWire,'a');
		i2c_endTransmission(myLittleWire);
		i2c_requestFrom(myLittleWire,t,1,rxBuffer);
		if(rxBuffer[0]!=0)
		{
			addressFound=1;
			printf("Address: %d\n",t);
			for(index=0;index<8;index++)
				printf("%d-%X\n",index,rxBuffer[index]);
		}
	}
	if(!addressFound)
		printf("Address couldn't be found! There is an error.\n");*/

	unsigned char i=0;
	
	for(;;){ // Constantly change colors ...
		i2c_beginTransmission(myLittleWire,0);
		i2c_send(myLittleWire,'c');
		i2c_send(myLittleWire,i++);
		i2c_send(myLittleWire,i*3);
		i2c_send(myLittleWire,255-i);
		i2c_endTransmission(myLittleWire);
		delay(30);
	}
}
