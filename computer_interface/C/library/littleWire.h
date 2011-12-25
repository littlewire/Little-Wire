#ifndef LITTLEWIRE_H
#define LITTLEWIRE_H

/*


	ihsan Kehribar
	November 2011


	http://kehibar.me/projects/Little-Wire



*/

/********************************************************************************
* Including header files
********************************************************************************/
#if defined(LINUX)
   #include <usb.h>			// this is libusb, see http://libusb.sourceforge.net/
#else
   #include <lusb0_usb.h>		// this is libusb, see http://libusb.sourceforge.net/
#endif
#include "opendevice.h"			// common code moved to separate module
/*******************************************************************************/

/********************************************************************************
* Useful constants and typedefs
********************************************************************************/
#define INPUT	1
#define OUTPUT	0

#define LOW	0
#define HIGH	1

typedef usb_dev_handle littleWire;
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
#define ADC0	0
#define ADC1	1
#define ADC2	2

// PWM Pins
#define PWM0	PIN0
#define PWM1	PIN1

// Aliases
#define PWMA	PWM0
#define PWMB	PWM1

#define SCK	PIN2
#define MISO	PIN1
#define MOSI	PIN0
#define RESET	PIN3
/*******************************************************************************/

/********************************************************************************
* Globals
********************************************************************************/
static int vendorID = 0x1781;
static int productID = 0x0c9f;
static int usbTimeout = 5000;

unsigned char rxBuffer[64];
/*******************************************************************************/


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
*     channel: 0 for RESET pin, 1 for SCK pin, 2 for internal Temperature sensor
*     Returns: Analog voltage in 10bit resoultion
********************************************************************************/
unsigned int analogRead(littleWire* lwHandle, unsigned char channel);
/*******************************************************************************/

/********************************************************************************
* Initialize the Pwm module on the device
********************************************************************************/
void initPwm(littleWire* lwHandle);
/*******************************************************************************/

/********************************************************************************
* Update the compare values of Pwm outputs
*     channelA: Compare value of Channel A
*     channelB: Compare value of Channel B
/*******************************************************************************/
void updatePwmCompare(littleWire* lwHandle, unsigned char channelA, unsigned char channelB);
/*******************************************************************************/

/********************************************************************************
* Change the Pwm prescaler. Default: 1024
*     value: 1024/256/64/8/1
********************************************************************************/
void updatePwmPrescale(littleWire* lwHandle, unsigned int value);
/*******************************************************************************/

/********************************************************************************
* Send one byte SPI message. Chip select is manual.
*    message: Message to send
*    Returns: Received SPI message
********************************************************************************/
unsigned char sendSpiMessage(littleWire* lwHandle, unsigned char message);
/*******************************************************************************/

#endif
