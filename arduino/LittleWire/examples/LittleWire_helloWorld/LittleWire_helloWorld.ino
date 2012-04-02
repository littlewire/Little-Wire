/*
  Printf style debug over AVR-ISP pins using Little Wire.
  Use the same cable for debugging and ISP!
  	
  http://kehribar.me/projects/Little-Wire/
  ihsan Kehribar <ihsan@kehribar.me> 
*/

#include <LittleWire.h>
LittleWire lw;

uint8_t txBuffer[32];
uint8_t i=0;
uint8_t length=0;

void setup()
{
  lw.begin();
  length=sprintf((char*)txBuffer,"Hello World!\n");
  lw.send(txBuffer,length);
}

void loop()
{
  while(lw.isBusy());
  length=sprintf((char*)txBuffer,"Count:%d\n",i++);
  lw.send(txBuffer,length);
}

