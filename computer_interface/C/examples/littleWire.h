/*










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
* Special definitions
********************************************************************************/
#define SCK_pin	2
#define MISO_pin 1
#define MOSI_pin 0
#define RESET_pin 5
// static int  vendorID = 0x1781;
// static int  productID = 0x0c9f;
// static int  usbTimeout = 5000;
unsigned char rxBuffer[64];
/*******************************************************************************/

/********************************************************************************
* Try to connect to the device
*     Returns: littleWirePointer for success, 0 for fail
********************************************************************************/
usb_dev_handle* littleWire_connect();
/*******************************************************************************/

/********************************************************************************
* Set a GPIO pin High/Low
*     pin: Pin number
*     state: 1 for High, 0 for Low
********************************************************************************/
void digitalWrite(usb_dev_handle* littleWirePointer,unsigned char pin, unsigned char state);
/*******************************************************************************/

/********************************************************************************
* Set a GPIO pin input/output
*     pin: Pin number
*     mod: 1 for input, 0 for output
********************************************************************************/
void pinMode(usb_dev_handle* littleWirePointer,unsigned char pin, unsigned char mod);
/*******************************************************************************/

/********************************************************************************
* Read a state of a GPIO pin
*     pin: Pin number
*     Returns: 1 for HIGH, 0 for LOW
********************************************************************************/
unsigned char digitalRead(usb_dev_handle* littleWirePointer,unsigned char pin);
/*******************************************************************************/

/********************************************************************************
* Read analog voltage from a spesific channel
*     channel: 0 for RESET pin, 1 for SCK pin, 2 for internal Temperature sensor
*     Returns: Analog voltage in 10bit resoultion
********************************************************************************/
unsigned int analogRead(usb_dev_handle* littleWirePointer,unsigned char channel);
/*******************************************************************************/

/********************************************************************************
* Initialize the Pwm module on the device
********************************************************************************/
void initPwm(usb_dev_handle* littleWirePointer);
/*******************************************************************************/

/********************************************************************************
* Update the compare values of Pwm outputs
*     channelA: Compare value of Channel A
*     channelB: Compare value of Channel B
/*******************************************************************************/
void updatePwmCompare(usb_dev_handle* littleWirePointer,unsigned char channelA, unsigned char channelB);
/*******************************************************************************/

/********************************************************************************
* Change the Pwm prescaler. Default: 1024
*     value: 1024/256/64/8/1
********************************************************************************/
void updatePwmPrescale(usb_dev_handle* littleWirePointer,unsigned int value);
/*******************************************************************************/

/********************************************************************************
* Send one byte SPI message. Chip select is manual.
*    message: Message to send
*    Returns: Received SPI message
********************************************************************************/
unsigned char sendSpiMessage(usb_dev_handle* littleWirePointer,unsigned char message);
/*******************************************************************************/       
