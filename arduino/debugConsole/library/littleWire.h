#ifndef LITTLEWIRE_H
#define LITTLEWIRE_H

/*
	http://kehribar.me/projects/Little-Wire/

	Copyright (C) <2011> ihsan Kehribar <ihsan@kehribar.me>
	Modified by Omer Kilic <omerkilic@gmail.com>

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

/********************************************************************************
* Header files
********************************************************************************/
#if defined(LINUX)
   #include <usb.h>				// this is libusb, see http://libusb.sourceforge.net/
#else
   #include <lusb0_usb.h>		// this is libusb, see http://libusb.sourceforge.net/
#endif
#include "opendevice.h"			// common code moved to separate module
/*******************************************************************************/

/********************************************************************************
* USB details
********************************************************************************/
#define	VENDOR_ID	0x1781
#define	PRODUCT_ID	0x0c9f
#define USB_TIMEOUT	5000
#define RX_BUFFER_SIZE	64
/*******************************************************************************/

/********************************************************************************
* Useful constants
********************************************************************************/
#define INPUT	1
#define OUTPUT	0

#define LOW		0
#define HIGH	1

#define AUTO_CS		1
#define MANUAL_CS	0
/*******************************************************************************/

/********************************************************************************
* Pin definitions
********************************************************************************/
// General Purpose Pins
#define PIN1	1
#define PIN2	2
#define PIN3	5
#define PIN4	0

// ADC Channels
#define ADC_PIN3		0
#define ADC_PIN2		1
#define ADC_TEMP_SENS	2

// PWM Pins
#define PWM1	PIN4
#define PWM2	PIN1

// Aliases
#define ADC0	ADC_PIN3
#define ADC1	ADC_PIN2
#define ADC2	ADC_TEMP_SENS
#define PWMA	PWM1
#define PWMB	PWM2

// 'AVR ISP' Pins
#define SCK_PIN		PIN2
#define MISO_PIN	PIN1
#define MOSI_PIN	PIN4
#define RESET_PIN	PIN3
/*******************************************************************************/

// Typedefs
typedef usb_dev_handle littleWire;
unsigned char rxBuffer[RX_BUFFER_SIZE]; /* This has to be unsigned for the data's sake */

// Function Prototypes

/********************************************************************************
* Try to connect to the device
*     Returns: lwHandle for success, 0 for fail
********************************************************************************/
littleWire* littleWire_connect();
/*******************************************************************************/

/********************************************************************************
* Set a GPIO pin High/Low
*     pin: Pin number
*     state: 1 for High, 0 for Low
********************************************************************************/
void digitalWrite(littleWire* lwHandle, unsigned char pin, unsigned char state);
/*******************************************************************************/

/********************************************************************************
* Set a GPIO pin input/output
*     pin: Pin number
*     mode: 1 for input, 0 for output
********************************************************************************/
void pinMode(littleWire* lwHandle, unsigned char pin, unsigned char mode);
/*******************************************************************************/

/********************************************************************************
* Read a state of a GPIO pin
*     pin: Pin number
*     Returns: 1 for HIGH, 0 for LOW
********************************************************************************/
unsigned char digitalRead(littleWire* lwHandle, unsigned char pin);
/*******************************************************************************/

/********************************************************************************
* Read analog voltage from a spesific channel
*     channel: 0 for pin3, 1 for pin2, 2 for internal Temperature sensor
*     Returns: Analog voltage in 10bit resoultion
********************************************************************************/
unsigned int analogRead(littleWire* lwHandle, unsigned char channel);
/*******************************************************************************/

/********************************************************************************
* Initialize the Pwm module on the device
********************************************************************************/
void pwm_init(littleWire* lwHandle);
/*******************************************************************************/

/********************************************************************************
* Stop the PWM module on the device
********************************************************************************/
void pwm_stop(littleWire* lwHandle);
/*******************************************************************************/

/********************************************************************************
* Update the compare values of Pwm outputs
*     channelA: Compare value of Channel A
*     channelB: Compare value of Channel B
/*******************************************************************************/
void pwm_updateCompare(littleWire* lwHandle, unsigned char channelA, unsigned char channelB);
/*******************************************************************************/

/********************************************************************************
* Change the Pwm prescaler. Default: 1024
*     value: 1024/256/64/8/1
********************************************************************************/
void pwm_updatePrescaler(littleWire* lwHandle, unsigned int value);
/*******************************************************************************/

/********************************************************************************
* Initialize SPI module
********************************************************************************/
void spi_init(littleWire* lwHandle);
/*******************************************************************************/

/********************************************************************************
* Send one byte SPI message. Chip select is manual.
*    message: Message to send
*    Returns: Received SPI message
********************************************************************************/
unsigned char spi_sendMessage(littleWire* lwHandle, unsigned char message);
/*******************************************************************************/

/********************************************************************************
* Send multiple SPI messages. Chip select is manual/automatic.
*    sendBuffer: Message array to send
*    inputBuffer: Returned answer message
*	 length: Message length - maximum 4
*	 mode: 1 for auto chip select , 0 for manual
********************************************************************************/
void spi_sendMessage_multiple(littleWire* lwHandle, unsigned char * sendBuffer, unsigned char * inputBuffer, unsigned char length ,unsigned char mode);
/*******************************************************************************/

/********************************************************************************
* Update SPI signal delay amount. Tune if neccessary to fit your requirements.
*	duration: Delay amount.
********************************************************************************/
void spi_updateDelay(littleWire* lwHandle, unsigned int duration);
/*******************************************************************************/

/********************************************************************************
* Initialize i2c module on Little-Wire
********************************************************************************/
void i2c_init(littleWire* lwHandle);
/*******************************************************************************/

/********************************************************************************
* Start the i2c tranmission
*	address: Slave device address
********************************************************************************/
void i2c_beginTransmission(littleWire* lwHandle, unsigned char address);
/*******************************************************************************/

/********************************************************************************
* Add new byte to the i2c send buffer
*	message: A byte to send.
********************************************************************************/
void i2c_send(littleWire* lwHandle,unsigned char message);
/*******************************************************************************/

/********************************************************************************
* Send the whole message buffer to the slave at once and end the tranmssion.
********************************************************************************/
void i2c_endTransmission(littleWire* lwHandle);
/*******************************************************************************/

/********************************************************************************
* Request a reply / message from a slave device.
*	address: Slave address
*	numBytes: Number of bytes the slave will send.
*	responseBuffer: Array pointer which will hold the response from the slave
********************************************************************************/
void i2c_requestFrom(littleWire* lwHandle,unsigned char address,unsigned char numBytes,unsigned char * responseBuffer);
/*******************************************************************************/

unsigned char debug_rw(littleWire* lwHandle, unsigned char msg, unsigned char delay);

#endif
