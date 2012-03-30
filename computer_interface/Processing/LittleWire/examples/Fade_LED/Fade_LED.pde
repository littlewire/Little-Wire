/*
 PWM example for Little Wire
 Connect a LED to the PIN4
 
 April 2012 - ihsan Kehribar
 http://kehribar.me/projects/little-wire
 */

import cc.littleWire.*;

LittleWire lw;

boolean connected=false;
boolean state=true;
int value=0;

void setup()
{
  lw = new LittleWire();
  connected=lw.Connect();
  if (connected)
  {  
   lw.pwm_init(); 
   lw.pwm_updatePrescaler(1);
  }
  size(320, 240);
  background(0, 0, 0);
}

void draw()
{
	if(value==255)
		state=false;
	else if(value==0)
		state=true;

	if(state) /* Ascending */
	{
		value++;
		lw.pwm_updateCompare(value,0);
		background(value,0,0);
	}
	else /* Descending */
	{
		value--;
		lw.pwm_updateCompare(value,0);
		background(value,0,0);
	}	
}
