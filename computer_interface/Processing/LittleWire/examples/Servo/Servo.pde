/*
 Servo example for Little Wire
 Connect a hobby servo to PIN1
 
 April 2012 - ihsan Kehribar
 http://kehribar.me/projects/little-wire
 */

import cc.littleWire.*;

LittleWire lw;

boolean connected=false;
boolean state=true;
int value=0;
int version;

void setup()
{
  lw = new LittleWire();
  connected=lw.Connect();
  if (connected)
  {  
   lw.servo_init();
   version=lw.readFirmwareVersion();
   System.out.printf("Little Wire firmware version: %d.%d\n",((version & 0xF0)>>4),(version&0x0F));	   
  }
  size(320, 240);
  background(0, 0, 0);
}

void draw()
{
	if(value==180)
		state=false;
	else if(value==0)
		state=true;

	if(state) /* Ascending */
	{
		value++;
		lw.servo_updateLocation(0,value);
		background(value,0,0);
	}
	else /* Descending */
	{
		value--;
		lw.servo_updateLocation(0,value);
		background(value,0,0);
	}
	print("Current location: ");
	println(value);
}
