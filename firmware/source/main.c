/* Name: main.c

  modified by ihsan Kehribar, 2011 November
  
  created by chris chung, 2010 April

  based on the works found in

  v-usb framework http://www.obdev.at/vusb/
	 Project: Thermostat based on AVR USB driver
	 Author: Christian Starkjohann
    
  usbtiny isp http://www.xs4all.nl/~dicks/avr/usbtiny/
  	Dick Streefland
  
  please observe licensing term from the above two projects

	Copyright (C) 2010  chris chung

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

 */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>

#include "usbdrv.h"
#include "twi_i2c/TinyWireM.h"
#include "oddebug.h"
#define sbi(register,bit) (register|=(1<<bit))
#define cbi(register,bit) (register&=~(1<<bit))
enum
{
	// Generic requests
	USBTINY_ECHO,		// echo test	0
	USBTINY_READ,		// read byte (wIndex:address)	1
	USBTINY_WRITE,		// write byte (wIndex:address, wValue:value)	2
	USBTINY_CLR,		// clear bit (wIndex:address, wValue:bitno)	3
	USBTINY_SET,		// set bit (wIndex:address, wValue:bitno)	4
	// Programming requests
	USBTINY_POWERUP,	// apply power (wValue:SCK-period, wIndex:RESET)	5
	USBTINY_POWERDOWN,	// remove power from chip	6
	USBTINY_SPI,		// issue SPI command (wValue:c1c0, wIndex:c3c2)	7
	USBTINY_POLL_BYTES,	// set poll bytes for write (wValue:p1p2)	8
	USBTINY_FLASH_READ,	// read flash (wIndex:address)	9
	USBTINY_FLASH_WRITE,	// write flash (wIndex:address, wValue:timeout)	10
	USBTINY_EEPROM_READ,	// read eeprom (wIndex:address)	11
	USBTINY_EEPROM_WRITE,	// write eeprom (wIndex:address, wValue:timeout)	12
	// Additional requests - ihsanKehribar
	USBTINY_PIN_SET_INPUT,	//	13
	USBTINY_PIN_SET_OUTPUT, //	14
	USBTINY_READ_ADC,	//	15
	USBTINY_SETUP_PWM, //16
	USBTINY_UPDATE_PWM_COMPARE, // 17
	USBTINY_PIN_SET_HIGH, // 18
	USBTINY_PIN_SET_LOW, // 19
	USBTINY_PIN_READ, // 20
	USBTINY_SINGLE_SPI, // 21
	USBTINY_CHANGE_PWM_PRESCALE, // 22
	USBTINY_SETUP_SPI, // 23
	USBTINY_SETUP_I2C, // 24
	USBTINY_I2C_BEGIN_TX, // 25
	USBTINY_I2C_ADD_BUFFER, // 26
	USBTINY_I2C_SEND_BUFFER, // 27
	USBTINY_SPI_ADD_BUFFER, // 28
	USBTINY_SPI_SEND_BUFFER // 29
};

#define	PORT	PORTB
#define	DDR		DDRB
#define	PIN		PINB

//
// to reduce pin count so that this can fit in a 8 pin tiny
// . no power nor ground pins to target, they are to be connected always
// . no reset control pin to target, target reset always grounded
//   * this had caused problem and there are two solutions
//     1. provide a toggle switch to off-on-off target reset to ground
//     2. introduce reset control and use reset pin as io
//
#define	POWER_MASK	0x00
#define	GND_MASK	0x00

#define	RESET_MASK	(1 << 5)
#define	SCK_MASK	(1 << 2)
#define	MISO_MASK	(1 << 1)
#define	MOSI_MASK	(1 << 0)

// ----------------------------------------------------------------------
// Local data
// ----------------------------------------------------------------------
static	uchar		sck_period=40;	// SCK period in microseconds (1..250)
static	uchar		poll1;		// first poll byte for write
static	uchar		poll2;		// second poll byte for write
static	unsigned		address;	// read/write address
static	unsigned		timeout;	// write timeout in usec
static	uchar		cmd0;		// current read/write command byte
static	uchar		cmd[4];		// SPI command buffer
static	uchar		res[4];		// SPI result buffer
#define SPI_BUFFER_SIZE 16 
static uint8_t spiBuffer[SPI_BUFFER_SIZE];
static uint8_t spiBuffer_count=0;
static uint8_t i=0;

// ----------------------------------------------------------------------
// Delay exactly <sck_period> times 0.5 microseconds (6 cycles).
// ----------------------------------------------------------------------
__attribute__((always_inline))
static	void	delay ( void )
{
	asm volatile(
		"	mov	__tmp_reg__,%0	\n"
		"0:	rjmp	1f		\n"
		"1:	nop			\n"
		"2:	nop			\n"
		"3:	nop			\n"
		"	dec	__tmp_reg__	\n"
		"	brne	0b		\n"
		: : "r" (sck_period) );
}

// ----------------------------------------------------------------------
// Issue one SPI command.
// ----------------------------------------------------------------------
static	void	spi ( uchar* cmd, uchar* res )
{
	uchar	i;
	uchar	c;
	uchar	r;
	uchar	mask;

	for	( i = 0; i < 4; i++ )
	{
		c = *cmd++;
		r = 0;
		for	( mask = 0x80; mask; mask >>= 1 )
		{
			if	( c & mask )
			{
				PORT |= MOSI_MASK;
			}
			delay();
			PORT |= SCK_MASK;
			delay();
			r <<= 1;
			if	( PIN & MISO_MASK )
			{
				r++;
			}
			PORT &= ~MOSI_MASK;
			PORT &= ~SCK_MASK;
		}
		*res++ = r;
	}
}

// ----------------------------------------------------------------------
// Create and issue a read or write SPI command.
// ----------------------------------------------------------------------
static	void	spi_rw ( void )
{
	unsigned	a;

	a = address++;
	if	( cmd0 & 0x80 )
	{	// eeprom
		a <<= 1;
	}
	cmd[0] = cmd0;
	if	( a & 1 )
	{
		cmd[0] |= 0x08;
	}
	cmd[1] = a >> 9;
	cmd[2] = a >> 1;
	spi( cmd, res );
}

// ----------------------------------------------------------------------
// Handle an IN packet.
// ----------------------------------------------------------------------
uchar usbFunctionRead(uchar *data, uchar len)
{
	uchar	i;

	for	( i = 0; i < len; i++ )
	{
		spi_rw();
		data[i] = res[3];
	}
	return len;
}

// ----------------------------------------------------------------------
// Handle an OUT packet.
// ----------------------------------------------------------------------
uchar usbFunctionWrite(uchar *data, uchar len)
{
	uchar	i;
	unsigned	usec;
	uchar	r;
	//uchar	last = (len != 8);

	for	( i = 0; i < len; i++ )
	{
		cmd[3] = data[i];
		spi_rw();
		cmd[0] ^= 0x60;	// turn write into read
		//
		for	( usec = 0; usec < timeout; usec += 32 * sck_period )
		{	// when timeout > 0, poll until byte is written
			spi( cmd, res );
			r = res[3];
			if	( r == cmd[3] && r != poll1 && r != poll2 )
			{
				break;
			}
		}
		//
	}
	//return last;
	return 1;
}

/* ------------------------------------------------------------------------- */
/* ------------------------ interface to USB driver ------------------------ */
/* ------------------------------------------------------------------------- */

uchar	usbFunctionSetup(uchar data[8])
{
// ----------------------------------------------------------------------
// Handle a non-standard SETUP packet.
// ----------------------------------------------------------------------
	uchar	bit;
	uchar	mask;
	uchar*	addr;
	uchar	req;

	// Generic requests
	req = data[1];
	if	( req == USBTINY_ECHO )
	{
		data[1]=0x21;
		usbMsgPtr = data;
		return 8;
	}
	addr = (uchar*) (int) data[4];
	if	( req == USBTINY_READ )
	{
		data[0] = PIN;
		usbMsgPtr = data;
		return 1;
	}
	if	( req == USBTINY_WRITE )
	{
		PORT = data[2];
		return 0;
	}
	bit = data[2] & 7;
	mask = 1 << bit;
	if	( req == USBTINY_CLR )
	{
		PORT &= ~ mask;
		return 0;
	}
	if	( req == USBTINY_SET )
	{
		PORT |= mask;
		return 0;
	}

	// Programming requests
	if	( req == USBTINY_POWERUP )
	{
		sck_period = data[2];
		mask = POWER_MASK;
		if	( data[4] )
		{
			mask |= RESET_MASK;
		}
		DDR  &= ~MISO_MASK;
		DDR  |= (RESET_MASK|SCK_MASK|MOSI_MASK);
		PORT &= ~(RESET_MASK|SCK_MASK|MOSI_MASK|MISO_MASK);
		return 0;
	}
	if	( req == USBTINY_POWERDOWN )
	{
		//PORT |= RESET_MASK;
		//DDR  &= ~(SCK_MASK|MOSI_MASK);
		DDRB  = RESET_MASK;
		PORTB = RESET_MASK;
		return 0;
	}
	/* have to remove the following check as we strip a lot of io
	if	( ! PORT )
	{
		return 0;
	}
	*/
	if	( req == USBTINY_SPI )
	{
		spi( data + 2, data + 0 );
		usbMsgPtr = data;
		return 4;
	}
	if	( req == USBTINY_POLL_BYTES )
	{
		poll1 = data[2];
		poll2 = data[3];
		return 0;
	}
	address = * (unsigned*) & data[4];
	if	( req == USBTINY_FLASH_READ )
	{
		cmd0 = 0x20;
		return 0xff;	// usb_in() will be called to get the data
	}
	if	( req == USBTINY_EEPROM_READ )
	{
		cmd0 = 0xa0;
		return 0xff;	// usb_in() will be called to get the data
	}
	timeout = * (unsigned*) & data[2];
	if	( req == USBTINY_FLASH_WRITE )
	{
		cmd0 = 0x40;
		return 0xff;	// data will be received by usb_out()
	}
	if	( req == USBTINY_EEPROM_WRITE )
	{
		cmd0 = 0xc0;
		return 0xff;	// data will be received by usb_out()
	}
	
	// - ihsan Kehribar -
	
	if( req == USBTINY_PIN_SET_INPUT ) // 13
	{
		DDR &= ~(1<<bit);
		return 0;
	}
	if( req == USBTINY_PIN_SET_OUTPUT ) // 14
	{
		DDR |= (1<<bit);
		return 0;
	}
	if( req == USBTINY_READ_ADC ) // 15
	{
		unsigned char temp=DDR;
		if(data[2]==0) // Read ADC from RESET pin
		{
			ADMUX=0;
			DDR &= ~(1<<5);
		}
		else if(data[2]==1) // Read ADC from SCK pin
		{
			ADMUX=1;
			DDR &= ~(1<<2);
		}
		else if(data[2]==2) // Read ADC from internal Temperature sensor
		{
			ADMUX=0b10001111;
		}
		ADCSRA|=(1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		data[0]=(unsigned char)ADCL;
		data[1]=(unsigned char)ADCH;
		usbMsgPtr = data;
		DDR=temp;		
		return 2;
	}
	if( req == USBTINY_SETUP_PWM ) // 16
	{
		DDR |= (1<<0); // Set PORTB0 Output
		DDR |= (1<<1); // Set PORTB1 Output
		TCCR0A |= (1<<COM0A1)|(0<<COM0A0)|(1<<COM0B1)|(0<<COM0B0); // Clear OC0A/OC0B on Compare Match, set OC0A/OC0B at BOTTOM (non-inverting mode)
		TCCR0A |= (1<<WGM01)|(1<<WGM00); // Fast PWM mode
		TCCR0B |= (1<<CS02)|(0<<CS01)|(1<<CS00); // Timer prescale: 1024 => PWM signal update frequency: ( 16.500.000 / ( 255 * 1024) ) = ~ 63 Hertz
		return 0;
	}
	if( req == USBTINY_UPDATE_PWM_COMPARE ) // 17
	{
		OCR0A=data[2];
		OCR0B=data[4];
		return 0;
	}
	if( req == USBTINY_PIN_SET_HIGH ) // 18
	{
		PORT |= (1<<bit);
		return 0;
	}
	if( req == USBTINY_PIN_SET_LOW ) // 19
	{
		PORT &= ~(1<<bit);
		return 0;
	}
	if( req == USBTINY_PIN_READ ) // 20
	{
		data[0]=(PIN&(1<<bit))>>bit;
		usbMsgPtr = data;
		return 1;
	}
	if( req == USBTINY_SINGLE_SPI ) // 21
	{
		char sendMessage=data[2];
		USIDR = sendMessage;
		USISR = (1<<USIOIF);
		cli();
		do {
			USICR = (1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
			_delay_us(10);
		} while ((USISR & (1<<USIOIF))==0);
		sei();
		data[0]=USIDR;
		usbMsgPtr=data;
		return 1;
	}
	if( req == USBTINY_CHANGE_PWM_PRESCALE ) // 22
	{
		if(data[2]==0) // ~ 64kHz update frequency 
		{
			 cbi(TCCR0B,CS02);
			 cbi(TCCR0B,CS01);
			 sbi(TCCR0B,CS00);
		}
		else if(data[2]==1) // ~ 8kHz Hertz update frequency
		{
			 cbi(TCCR0B,CS02);
			 sbi(TCCR0B,CS01);
			 cbi(TCCR0B,CS00);
		}
		else if(data[2]==2) // ~ 1 kHz update frequency
		{
			 cbi(TCCR0B,CS02);
			 sbi(TCCR0B,CS01);
			 sbi(TCCR0B,CS00);
		}		
		else if(data[2]==3) // ~ 252 Hertz update frequency
		{
			 sbi(TCCR0B,CS02);
			 cbi(TCCR0B,CS01);
			 cbi(TCCR0B,CS00);
		}
		else if(data[2]==4) // ~ 63 Hertz update frequency -> Suitable for Servo driving. -> Default
		{
			 sbi(TCCR0B,CS02);
			 cbi(TCCR0B,CS01);
			 sbi(TCCR0B,CS00);
		}			
		
		return 0;
	}
	if(req == USBTINY_SETUP_SPI) // 23
	{
		DDR |= (1<<1); // Data Out
		DDR &= ~(1<<0); // Data Input
		DDR |= (1<<2); // Clock output
		
		// data[2] indicates mode
		// data[4] indicates clock speed
		
		USICR = (1<<USIWM0)|(1<<USICS1)|(1<<USICLK);
		return 0;
	}
	if( req == USBTINY_SETUP_I2C ) // 24
	{
		i2c_begin();
		return 0;
	}
	if( req == USBTINY_I2C_BEGIN_TX ) // 25
	{
		// data[2] has the TX adress
		i2c_beginTransmission(data[2]);
		return 0;
	}
	if( req == USBTINY_I2C_ADD_BUFFER ) // 26
	{
		i2c_send(data[2]);
		return 0;
	}
	if( req == USBTINY_I2C_SEND_BUFFER) // 27
	{
		cli();
			i2c_endTransmission(); // Actually sends the whole buffer at once here.
		sei();
		return 0;
	}
	if( req == USBTINY_SPI_ADD_BUFFER) // 28
	{
		if(spiBuffer_count<SPI_BUFFER_SIZE)
		{
			spiBuffer[spiBuffer_count]=data[2];
			spiBuffer_count++;
		}
		return 0;
	}
	if( req == USBTINY_SPI_SEND_BUFFER) // 29
	{
		for(i=0;i<spiBuffer_count;i++)
		{
			USIDR = spiBuffer[i];
			USISR = (1<<USIOIF);
			cli();
			do {
				USICR = (1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
				_delay_us(10);
			} while ((USISR & (1<<USIOIF))==0);
			sei();
		}
		spiBuffer_count=0;
		return 0;
	}
	
	return 0;
}


/* ------------------------------------------------------------------------- */
/* ------------------------ Oscillator Calibration ------------------------- */
/* ------------------------------------------------------------------------- */

/* Calibrate the RC oscillator to 8.25 MHz. The core clock of 16.5 MHz is
 * derived from the 66 MHz peripheral clock by dividing. Our timing reference
 * is the Start Of Frame signal (a single SE0 bit) available immediately after
 * a USB RESET. We first do a binary search for the OSCCAL value and then
 * optimize this value with a neighboorhod search.
 * This algorithm may also be used to calibrate the RC oscillator directly to
 * 12 MHz (no PLL involved, can therefore be used on almost ALL AVRs), but this
 * is wide outside the spec for the OSCCAL value and the required precision for
 * the 12 MHz clock! Use the RC oscillator calibrated to 12 MHz for
 * experimental purposes only!
 */
static void calibrateOscillator(void)
{
uchar       step = 128;
uchar       trialValue = 0, optimumValue;
int         x, optimumDev, targetValue = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5);

    /* do a binary search: */
    do{
        OSCCAL = trialValue + step;
        x = usbMeasureFrameLength();    /* proportional to current real frequency */
        if(x < targetValue)             /* frequency still too low */
            trialValue += step;
        step >>= 1;
    }while(step > 0);
    /* We have a precision of +/- 1 for optimum OSCCAL here */
    /* now do a neighborhood search for optimum value */
    optimumValue = trialValue;
    optimumDev = x; /* this is certainly far away from optimum */
    for(OSCCAL = trialValue - 1; OSCCAL <= trialValue + 1; OSCCAL++){
        x = usbMeasureFrameLength() - targetValue;
        if(x < 0)
            x = -x;
        if(x < optimumDev){
            optimumDev = x;
            optimumValue = OSCCAL;
        }
    }
    OSCCAL = optimumValue; 
}

void    usbEventResetReady(void)
{
    calibrateOscillator();
    eeprom_write_byte(0, OSCCAL);   /* store the calibrated value in EEPROM */
}

/* ------------------------------------------------------------------------- */
/* --------------------------------- main ---------------------------------- */
/* ------------------------------------------------------------------------- */

int main(void) {
	uchar   i;
	uchar   calibrationValue;

	//DDRB  = (RESET_MASK|SCK_MASK|MOSI_MASK);
	DDRB  = RESET_MASK;
	PORTB = RESET_MASK;
	/*
	_delay_ms(25);
	uchar pgm[] = { 0xac, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
	spi(pgm, pgm+4);
	*/
    calibrationValue = eeprom_read_byte(0); /* calibration value from last time */
    if(calibrationValue != 0xff){
        OSCCAL = calibrationValue;
    }
    odDebugInit();
    usbDeviceDisconnect();
    for(i=0;i<20;i++){  /* 300 ms disconnect */
        _delay_ms(15);
    }
    usbDeviceConnect();

    wdt_enable(WDTO_1S);

    usbInit();
    sei();
		
	// ADC init	
	ADMUX|=(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(1<<MUX0);
	ADMUX|=(0<<REFS2)|(0<<REFS1)|(0<<REFS0) |(0<<ADLAR);
	ADCSRA|= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	ADCSRA|=(1<<ADEN);
	ADCSRA|=(1<<ADSC);
	MCUCR|=(1<<PUD);
	while(ADCSRA & (1<<ADSC));
		
    for(;;)
	{   
        wdt_reset();
        usbPoll();
    }
    return 0;
}
