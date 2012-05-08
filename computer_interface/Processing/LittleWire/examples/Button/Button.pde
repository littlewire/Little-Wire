/*
 Button / digitalRead example for Little Wire
 Connect a button to PIN3 - (Active low)
 
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
    lw.pwm_stop();
    lw.pinMode(lw.PIN3, lw.INPUT);
    version=lw.readFirmwareVersion();
    System.out.printf("Little Wire firmware version: %d.%d\n",((version & 0xF0)>>4),(version&0x0F));	  	
  }
  size(320, 240);
  background(0, 0, 0);
}

void draw()
{
  if (lw.digitalRead(lw.PIN3)==lw.LOW) 
  {
    delay(100); /* Debouncing delay */
    if (lw.digitalRead(lw.PIN3)==lw.LOW)
    {
      println("Button pressed!");
      state=!state;
    }
  }
  if (state)
    background(255, 0, 0);	
  else
    background(0, 0, 0);
}
