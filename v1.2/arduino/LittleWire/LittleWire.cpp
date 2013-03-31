/*
	- Little Wire - 
	Printf style debug over AVR-ISP pins using Little Wire.
	http://kehribar.me/projects/Little-Wire/

	Copyright (C) <2012> ihsan Kehribar <ihsan@kehribar.me>

	Permission is hereby granted, free of charge, to any person obtaining a copy of
	this software and associated documentation files (the "Software"), to deal in
	the Software without restriction, including without limitation the rights to
	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
	of the Software, and to permit persons to whom the Software is furnished to do
	so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include "LittleWire.h"
#include "Print.h"
#include <avr/interrupt.h>
#include <util/delay.h>

/* SPI transfer complete interrupt */
ISR(SPI_STC_vect) 
{
  if(lw_messageSent==0)
  {	
    SPDR = lw_txBuffer[lw_index++]; /* Send a char to the computer */       
	if(lw_index==lw_length)
    {
	  lw_index=0;
	  lw_messageSent=1;
	  SPCR=0;	  
    }
  }
}

/* Empty constructor */
LittleWire::LittleWire()
{
	// ...
}

/* Init pins and SPI interface */
void LittleWire::begin(void)
{
	setup_spi(SPI_MODE_3, SPI_MSB, SPI_INTERRUPT, SPI_SLAVE);	
	lw_index = 32;
	lw_length = 0;
	lw_messageSent = 1;	
	sei();
}

/* Send the new message buffer */
void LittleWire::print(uint8_t* buffer,uint8_t new_length)
{	
	while(lw_messageSent==0);
	lw_length=new_length+1;
	lw_txBuffer=buffer;
	lw_index=0;
	lw_messageSent = 0;
	SPCR = lw_register;
}

/* Return the state */
uint8_t LittleWire::isBusy(void)
{
	if(lw_messageSent)
		return 0;
	else
		return 1;
}

/* Ref: http://www.rocketnumbernine.com/2009/07/03/using-spi-on-an-avr-3 */
void LittleWire::setup_spi(uint8_t mode, int dord, int interrupt, uint8_t clock)
{
  // specify pin directions for SPI pins on port B
  if (clock == SPI_SLAVE) { // if slave SS and SCK is input
    DDRB &= ~(1<<SPI_MOSI_PIN); // input
    DDRB |= (1<<SPI_MISO_PIN); // output    
    DDRB &= ~(1<<SPI_SCK_PIN);// input
	DDRB &= ~(1<<SPI_SS_PIN); // input
  } 
  else {
    DDRB |= (1<<SPI_MOSI_PIN); // output
    DDRB &= ~(1<<SPI_MISO_PIN); // input
    DDRB |= (1<<SPI_SCK_PIN);// output
    DDRB |= (1<<SPI_SS_PIN);// output
  }
  lw_register = (1<<SPE) // enable SPI
      | (dord<<DORD) // LSB or MSB
        | (((clock != SPI_SLAVE) ? 1 : 0) <<MSTR) // Slave or Master
          | (((mode & 0x02) == 2) << CPOL) // clock timing mode CPOL
              | (((mode & 0x01)) << CPHA) // clock timing mode CPHA
                | (((clock & 0x02) == 2) << SPR1) // cpu clock divisor SPR1
                  | ((clock & 0x01) << SPR0) // cpu clock divisor SPR0
					| (((interrupt == 1) ? 1: 0) << SPIF);
  SPSR = (((clock & 0x04) == 4) << SPI2X); // clock divisor SPI2X
}