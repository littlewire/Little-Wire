#ifndef LITTLEWIRE_H
#define LITTLEWIRE_H

/*
	http://kehribar.me/projects/Little-Wire/
	
	Copyright (C) <2011> ihsan Kehribar <ihsan@kehribar.me>
	Copyright (C) <2011> Omer Kilic <omerkilic@gmail.com>
	
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

extern "C" {
  #if defined(LINUX)
    #include <usb.h>			// this is libusb, see http://libusb.sourceforge.net/ 
  #else
    #include <lusb0_usb.h>		// this is libusb, see http://libusb.sourceforge.net/ 
  #endif

  #include "opendevice.h"
}

//namespace LittleWire
//{

/********************************************************************************
* Globals
********************************************************************************/
#define	VENDOR_ID	0x1781
#define	PRODUCT_ID	0x0c9f
#define USB_TIMEOUT	5000
#define	RX_BUFFER_SIZE	64
/*******************************************************************************/

/********************************************************************************
* Useful constants
********************************************************************************/
#define INPUT	1
#define OUTPUT	0

#define LOW	0
#define HIGH	1
/*******************************************************************************/

/********************************************************************************
* Pin definitions
********************************************************************************/
// General Purpose Pins
#define PIN0	0
#define PIN1	1
#define PIN2	2
#define PIN3	5

// ADC Channels
#define ADC_RESET_PIN	0
#define ADC_SCK_PIN	1
#define ADC_TEMP_SENS	2

// PWM Pins
#define PWM0	PIN0
#define PWM1	PIN1

// Aliases
#define ADC0	ADC_RESET_PIN
#define ADC1	ADC_SCK_PIN
#define ADC2	ADC_TEMP_SENS
#define PWMA	PWM0
#define PWMB	PWM1

// 'AVR ISP' Pins
#define SCK_PIN		PIN2
#define MISO_PIN	PIN1
#define MOSI_PIN	PIN0
#define RESET_PIN	PIN3
/*******************************************************************************/

class littleWire
{
//	struct usb_dev_handle;
	char rxBuffer[RX_BUFFER_SIZE];
	usb_dev_handle* lwHandle;

public:


	// Constructor
	littleWire();

	// Pin Functions
	void pinMode(unsigned char pin, unsigned char mode);
	void digitalWrite(unsigned char pin, unsigned char state);
	unsigned char digitalRead(unsigned char pin);
	unsigned int analogRead(unsigned char channel);

	// PWM Functions
	void pwm_init();
	void pwm_stop();
	void pwm_updateCompare(unsigned char channelA, unsigned char channelB);
	void pwm_updatePrescaler(unsigned int value);

	// SPI Functions
	void spi_init();
	unsigned char spi_sendMessage(unsigned char message);
	void spi_sendMessageMulti(unsigned char* sendBuffer, unsigned char* inputBuffer, unsigned char length, unsigned char mode);
	void spi_updateDelay(unsigned int duration);

	// I2C Functions
	void i2c_init();
	void i2c_beginTransmission(unsigned char address);
	void i2c_send(unsigned char message);
	void i2c_endTransmission();
	void i2c_requestFrom(unsigned char address,unsigned char numBytes,unsigned char* responseBuffer);

};

//} //namespace

#endif

