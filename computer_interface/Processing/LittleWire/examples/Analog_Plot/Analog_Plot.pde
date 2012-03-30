/*
 Basic analog voltage reading and 
 plotting example for Little Wire.
 
 Connect a potentiometer to PIN2
 
 April 2012 - ihsan Kehribar
 http://kehribar.me/projects/little-wire
 */

import cc.littleWire.*;

LittleWire lw;

PFont myFont;
int stepSize=3;
int x_axis=0;
int previous_y=0;
int actual_y;
int voltage=0;
boolean connected=false;
byte i=0;

void setup()
{
  lw = new LittleWire();
  connected=lw.Connect();
  if(connected)
  {
  	lw.pwm_stop();
  	lw.pinMode(lw.PIN2,lw.INPUT);
  }	
  size(800, 512);
  background(220);
  myFont = loadFont("AgencyFB-Reg-25.vlw");
  textFont(myFont, 32);
}

void draw()
{
	if(connected)
	{
	  voltage=lw.analogRead(lw.ADC_PIN2);
	  fill(255);
	  rect(5,5,90,45);
	  fill(0);
	  text((voltage*5/1024.0),15,40);
	  if (x_axis>800)
	  {
	    background(220);
	    x_axis=0;
	  }
	  actual_y=512-(voltage/2); // Because the zero location of y axis is at the left side
	  line(x_axis, previous_y, x_axis+stepSize,actual_y);
	  x_axis=x_axis+stepSize;
	  previous_y=actual_y;
	}
}
