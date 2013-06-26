/*
   Printf style debug over AVR-ISP pins using Little Wire.
   Use the same cable for debugging and ISP!
   
   Note: Tie down the SS pin (digital 10) to the ground.
   	
   http://kehribar.me/projects/Little-Wire/
   ihsan Kehribar <ihsan@kehribar.me> 
 */

#include <LittleWire.h>
LittleWire lw;

uint16_t analogValue;
char txBuffer[32];

void setup()
{
  lw.begin();
  lw.print("\n");
  lw.print("Prints analog_in #0 to console!\n");
  lw.print("Program started!\n");
  delay(2000);
}

void loop()
{ 
  analogValue=analogRead(0);
  sprintf(txBuffer,"Value: %d\n",analogValue);
  lw.print(txBuffer);
}





