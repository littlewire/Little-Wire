/*
	Updated for C++: August 2012
	Created: July 2012
	by ihsan Kehribar <ihsan@kehribar.me>
	
	Connect an RGB led to the PIN1, PIN2, PIN4
*/

#include <stdio.h>
#include <stdlib.h>
#include "littleWire.h"

unsigned char i=0;
int r,g,b;
int h,s,v;
unsigned char version;

// Taken from: http://www.nunosantos.net/archives/114
void HSVtoRGB( int  *r, int *g,int *b, int h, int s, int v );

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
	
	lw.softPWM_state(ENABLE);
	
	h=0;
	s=255;
	v=255;
	
	for(;;)
	{ 						
		if(h==359)
			h=0;
		else
			h+=1;
		
		HSVtoRGB(&r,&g,&b,h,s,v);		
		
		printf("> Current:\t#r: %3d\t#g: %3d\t#b: %3d\t\n",r,g,b);	
	
		lw.softPWM_write(r,g,b);

		delay(40);
		
		if(lw.isAlive()<0)
		{
			printf("> lwStatus: %d\n",lw.isAlive());
			printf("> Connection error!\n");
			return 0;
		}
	}
	
	return 0;
}

void HSVtoRGB( int  *r, int *g,int *b, int h, int s, int v )
{
        int f;
        long p, q, t;
 
        if( s == 0 )
        {
                *r = *g = *b = v;
                return;
        }
 
        f = ((h%60)*255)/60;
        h /= 60;
 
        p = (v * (256 - s))/256;
        q = (v * ( 256 - (s * f)/256 ))/256;
        t = (v * ( 256 - (s * ( 256 - f ))/256))/256;
 
        switch( h ) {
                case 0:
                        *r = v;
                        *g = t;
                        *b = p;
                        break;
                case 1:
                        *r = q;
                        *g = v;
                        *b = p;
                        break;
                case 2:
                        *r = p;
                        *g = v;
                        *b = t;
                        break;
                case 3:
                        *r = p;
                        *g = q;
                        *b = v;
                        break;
                case 4:
                        *r = t;
                        *g = p;
                        *b = v;
                        break;
                default:
                        *r = v;
                        *g = p;
                        *b = q;
                        break;
        }
}
 