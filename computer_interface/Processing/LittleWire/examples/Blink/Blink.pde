/*
 Blink a LED example for Little Wire.
 Connect a LED to the PIN4
 
 April 2012 - ihsan Kehribar
 http://kehribar.me/projects/little-wire
 */

import cc.littleWire.*;

LittleWire lw;

boolean connected=false;
boolean state=false;

void setup()
{
  lw = new LittleWire();
  connected=lw.Connect();
  if (connected)
  {
  	lw.pwm_stop();
    lw.pinMode(lw.PIN4, lw.OUTPUT);
  }
  size(320, 240);
  background(255, 0, 0);
}

void draw()
{
  if (connected)
  {
    if (state)
    {
      lw.digitalWrite(lw.PIN4, lw.HIGH);
      background(255, 0, 0);
      delay(500);
      state=false;
    }
    else
    {
      lw.digitalWrite(lw.PIN4, lw.LOW);
      background(0, 0, 0);
      delay(500);
      state=true;
    }
  }
}
