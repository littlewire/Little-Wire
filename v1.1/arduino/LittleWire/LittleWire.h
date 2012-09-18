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

#ifndef LittleWire_h
#define LittleWire_h

#include "Print.h"

extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
}


#if (defined(__AVR_AT90USB82__) || defined(__AVR_AT90USB162__))
	#define SPI_SS_PIN PORTB0
	#define SPI_SCK_PIN PORTB1
	#define SPI_MOSI_PIN PORTB2
	#define SPI_MISO_PIN PORTB3
#elif (defined(__AVR_ATmega48__) || defined(_AVR_ATmega88__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)||defined(__AVR_ATmega8__))
	#define SPI_SS_PIN PORTB2
	#define SPI_SCK_PIN PORTB5
	#define SPI_MOSI_PIN PORTB3
	#define SPI_MISO_PIN PORTB4
#else
	#error unknown processor - add to LittleWire.h
#endif

// SPI clock modes
#define SPI_MODE_0 0x00 /* Sample (Rising) Setup (Falling) CPOL=0, CPHA=0 */
#define SPI_MODE_1 0x01 /* Setup (Rising) Sample (Falling) CPOL=0, CPHA=1 */
#define SPI_MODE_2 0x02 /* Sample (Falling) Setup (Rising) CPOL=1, CPHA=0 */
#define SPI_MODE_3 0x03 /* Setup (Falling) Sample (Rising) CPOL=1, CPHA=1 */

// data direction
#define SPI_LSB 1 /* send least significant bit (bit 0) first */
#define SPI_MSB 0 /* send most significant bit (bit 7) first */

// whether to raise interrupt when data received (SPIF bit received)
#define SPI_NO_INTERRUPT 0
#define SPI_INTERRUPT 1

// slave or master with clock diviser
#define SPI_SLAVE 0xF0
#define SPI_MSTR_CLK4 0x00 /* chip clock/4 */
#define SPI_MSTR_CLK16 0x01 /* chip clock/16 */
#define SPI_MSTR_CLK64 0x02 /* chip clock/64 */
#define SPI_MSTR_CLK128 0x03 /* chip clock/128 */
#define SPI_MSTR_CLK2 0x04 /* chip clock/2 */
#define SPI_MSTR_CLK8 0x05 /* chip clock/8 */
#define SPI_MSTR_CLK32 0x06 /* chip clock/32 */

static volatile uint8_t*	lw_txBuffer;
static volatile uint8_t		lw_index;
static volatile uint8_t		lw_length;
static volatile uint8_t		lw_temp;
static volatile uint8_t		lw_messageSent;
static volatile uint8_t		lw_timeout;
static volatile uint16_t	lw_counter;
static volatile uint8_t		lw_register;

class LittleWire {
  private:
	void setup_spi(uint8_t mode, int dord, int interrupt, uint8_t clock);
  public:
	LittleWire();
	void begin(void);
	void print(uint8_t* buffer,uint8_t new_length);
	void print(const char *str) { return print((uint8_t *)str, strlen(str)); }
	void print(uint8_t* buffer) { return print((uint8_t *)buffer, strlen((const char*)buffer)); }
	void print(char* buffer) { return print((uint8_t *)buffer, strlen(buffer)); }
	uint8_t isBusy(void);
};

#endif

