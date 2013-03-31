#ifndef LITTLEWIRE_H
#define LITTLEWIRE_H

/*
	http://littlewire.cc

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
#include "littleWire_util.h"
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

#define ENABLE	1
#define DISABLE	0

#define HIGH	1
#define LOW		0

#define AUTO_CS		1
#define MANUAL_CS	0

// Voltage ref definition
#define VREF_VCC		0
#define VREF_1100mV		1
#define VREF_2560mV		2 

// I2C definition
#define END_WITH_STOP	1
#define NO_STOP			0
#define READ			1
#define WRITE			0
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

/********************************************************************************
* Typedef
********************************************************************************/
typedef usb_dev_handle littleWire;
/*******************************************************************************/

/********************************************************************************
* Global Variables
********************************************************************************/
unsigned char	rxBuffer[RX_BUFFER_SIZE]; /* This has to be unsigned for the data's sake */
unsigned char	ROM_NO[8];
int				lwStatus;
/*******************************************************************************/

/********************************************************************************
* Try to connect to the device
*     Returns: lwHandle for success, 0 for fail
********************************************************************************/
littleWire* littleWire_connect();
/*******************************************************************************/


/********************************************************************************
* Returns the firmware version
*	Format: 0xXY => X: Primary version	Y: Minor version
********************************************************************************/
unsigned char readFirmwareVersion(littleWire* lwHandle);
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
* Sets the state of the internal pullup resistor. Call this function after you
* assign the pin as an input.
*     pin: Pin number
*     state: ENABLE or DISABLE
********************************************************************************/
void internalPullup(littleWire* lwHandle, unsigned char pin, unsigned char state);
/*******************************************************************************/


/********************************************************************************
* Initialize the analog module. VREF_VCC is the standard voltage coming from the
* USB plug. Others are the Attiny's internal voltage references.
*     voltageRef: VREF_VCC or VREF_1100mV or VREF_2560mV
********************************************************************************/
void analog_init(littleWire* lwHandle, unsigned char voltageRef);
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
* Send SPI message(s). Chip select is manual. SPI Mode is 0. 
*    sendBuffer: Message array to send
*    inputBuffer: Returned answer message
*	 length: Message length - maximum 4
*	 mode: 1 for auto chip select , 0 for manual
********************************************************************************/
void spi_sendMessage(littleWire* lwHandle, unsigned char * sendBuffer, unsigned char * inputBuffer, unsigned char length ,unsigned char mode);
/*******************************************************************************/


/********************************************************************************
* Send one byte SPI message over MOSI pin. Slightly slower than the actual one.
* There isn't any chip select control involved. Useful for debug console.
*    message: Message to send
*    Returns: Received SPI message
********************************************************************************/
unsigned char debugSpi(littleWire* lwHandle, unsigned char message);
/*******************************************************************************/


/********************************************************************************
* Update SPI signal delay amount. Tune if neccessary to fit your requirements.
*	duration: Delay amount.
********************************************************************************/
void spi_updateDelay(littleWire* lwHandle, unsigned int duration);
/*******************************************************************************/


/********************************************************************************
* Initialize i2c module
********************************************************************************/
void i2c_init(littleWire* lwHandle);
/*******************************************************************************/


/********************************************************************************
* Start the i2c communication
* 	address7bit: 7bit address
*	direction: READ or WRITE
*	Returns: 1 if received ACK
********************************************************************************/
unsigned char i2c_start(littleWire* lwHandle, unsigned char address7bit, unsigned char direction);
/*******************************************************************************/


/********************************************************************************
* Sends byte(s) over i2c bus
* 	sendBuffer: Message array to send
*	length: Message length -> Max = 4
*	endWithStop: Should we send a STOP condition after this buffer? 
********************************************************************************/
void i2c_write(littleWire* lwHandle, unsigned char* sendBuffer, unsigned char length, unsigned char endWithStop);
/*******************************************************************************/


/********************************************************************************
* Read byte(s) over i2c bus
*	readBuffer: Returned message array
*	length: Message length -> Max = 8
*	endWithStop: Should we send a STOP condition after this buffer? 
********************************************************************************/
void i2c_read(littleWire* lwHandle, unsigned char* readBuffer, unsigned char length, unsigned char endWithStop);
/*******************************************************************************/


/********************************************************************************
* Update I2C signal delay amount. Tune if neccessary to fit your requirements.
*	duration: Delay amount.
********************************************************************************/
void i2c_updateDelay(littleWire* lwHandle, unsigned int duration);
/*******************************************************************************/


/********************************************************************************
* Send a single bit over onewire bus.
********************************************************************************/
void onewire_sendBit(littleWire* lwHandle, unsigned char bitValue);
/*******************************************************************************/


/********************************************************************************
* Send a byte over onewire bus.
********************************************************************************/
void onewire_writeByte(littleWire* lwHandle, unsigned char messageToSend);
/*******************************************************************************/


/********************************************************************************
* Read a byte over onewire bus.
********************************************************************************/
unsigned char onewire_readByte(littleWire* lwHandle);
/*******************************************************************************/


/********************************************************************************
* Read a single bit over onewire bus.
********************************************************************************/
unsigned char onewire_readBit(littleWire* lwHandle);
/*******************************************************************************/


/********************************************************************************
* Send a reset pulse over onewire bus.
*	Returns: If any device presents on the bus -> 1 ; otherwise -> 0
********************************************************************************/
unsigned char onewire_resetPulse(littleWire* lwHandle);
/*******************************************************************************/


/******************************************************************************
* Start searching for device adress on the onewire bus and store into them
* ROM_NO[] array. 
*	Returns: If no new device had found -> 0
********************************************************************************/
int onewire_firstAddress(littleWire* lwHandle);
/*******************************************************************************/


/********************************************************************************
* Find the next address and store them int ROM_NO[] array.
*	Returns: If no new device had found -> 0
* Taken from: http://www.maxim-ic.com/appnotes.cfm/appnote_number/187
/*******************************************************************************/
int onewire_nextAddress(littleWire* lwHandle);
/*******************************************************************************/


/********************************************************************************
* Set the state of the softPWM module
*	state: ENABLE or DISABLE
********************************************************************************/
void softPWM_state(littleWire* lwHandle,unsigned char state);
/*******************************************************************************/


/********************************************************************************
* Update the values of softPWM outputs.
********************************************************************************/
void softPWM_write(littleWire* lwHandle,unsigned char ch1,unsigned char ch2,unsigned char ch3);
/*******************************************************************************/


/********************************************************************************
* Send a custom control message ... 
********************************************************************************/
int customMessage(littleWire* lwHandle,unsigned char* receiveBuffer,unsigned char command,unsigned char d1,unsigned char d2, unsigned char d3, unsigned char d4);
/*******************************************************************************/


/********************************************************************************
* Returns an error code if there was an error.
********************************************************************************/
int littleWire_error ();
/*******************************************************************************/


/********************************************************************************
* Returns a text tescription of an error code.
********************************************************************************/
char *littleWire_errorName ();
/*******************************************************************************/

#endif
