/*


	ihsan Kehribar 
	November 2011

	
	http://kehibar.me/projects/Little-Wire



*/

#include "littleWire.h"

static int  vendorID = 0x1781;
static int  productID = 0x0c9f;
static int  usbTimeout = 5000;

/********************************************************************************
* Try to connect to the device
*     Returns: littleWirePointer for success, 0 for fail
********************************************************************************/
usb_dev_handle* littleWire_connect()
{
	usb_init();
	usb_dev_handle  *tempHandle = NULL;

	if ( usbOpenDevice(&tempHandle, vendorID, "*", productID, "*", "*", NULL, NULL ) )
	{
		printf("Little Wire could not be found!\n");
		return 0;
	}
	
	return tempHandle;
}
/*******************************************************************************/

/********************************************************************************
* Set a GPIO pin High/Low
*     pin: Pin number
*     state: 1 for High, 0 for Low
********************************************************************************/
void digitalWrite(usb_dev_handle* littleWirePointer,unsigned char pin, unsigned char state)
{
	if(state)
		usb_control_msg(littleWirePointer, 0xC0, 18, pin, 0, rxBuffer, 8, usbTimeout);
	else
		usb_control_msg(littleWirePointer, 0xC0, 19, pin, 0, rxBuffer, 8, usbTimeout);
}
/*******************************************************************************/

/********************************************************************************
* Set a GPIO pin input/output
*     pin: Pin number
*     mod: 1 for input, 0 for output
********************************************************************************/
void pinMode(usb_dev_handle* littleWirePointer,unsigned char pin, unsigned char mod)
{
	if(mod)
		usb_control_msg(littleWirePointer, 0xC0, 13, pin, 0, rxBuffer, 8, usbTimeout);
	else
		usb_control_msg(littleWirePointer, 0xC0, 14, pin, 0, rxBuffer, 8, usbTimeout);
}
/*******************************************************************************/

/********************************************************************************
* Read a state of a GPIO pin
*     pin: Pin number
*     Returns: 1 for HIGH, 0 for LOW
********************************************************************************/
unsigned char digitalRead(usb_dev_handle* littleWirePointer,unsigned char pin)
{
	usb_control_msg(littleWirePointer, 0xC0, 21, pin, 0, rxBuffer, 8, usbTimeout);
	return rxBuffer[0];
}
/*******************************************************************************/

/********************************************************************************
* Read analog voltage from a spesific channel
*     channel: 0 for RESET pin, 1 for SCK pin, 2 for internal Temperature sensor
*     Returns: Analog voltage in 10bit resoultion
********************************************************************************/
unsigned int analogRead(usb_dev_handle* littleWirePointer,unsigned char channel)
{	
	usb_control_msg(littleWirePointer, 0xC0, 15, channel, 0, rxBuffer, 8, usbTimeout);
	return ((rxBuffer[1] <<8) + (rxBuffer[0]));
}
/*******************************************************************************/

/********************************************************************************
* Initialize the Pwm module on the device
********************************************************************************/
void initPwm(usb_dev_handle* littleWirePointer)
{
	usb_control_msg(littleWirePointer, 0xC0, 16, 0, 0, rxBuffer, 8, usbTimeout);
}
/*******************************************************************************/

/********************************************************************************
* Update the compare values of Pwm outputs
*     channelA: Compare value of Channel A
*     channelB: Compare value of Channel B
/*******************************************************************************/
void updatePwmCompare(usb_dev_handle* littleWirePointer,unsigned char channelA, unsigned char channelB)
{
	usb_control_msg(littleWirePointer, 0xC0, 17, channelA, channelB, rxBuffer, 8, usbTimeout);
}
/*******************************************************************************/

/********************************************************************************
* Change the Pwm prescaler. Default: 1024
*     value: 1024/256/64/8/1
********************************************************************************/
void updatePwmPrescale(usb_dev_handle* littleWirePointer,unsigned int value)
{	
	switch(value)
	{
		case 1024:
			usb_control_msg(littleWirePointer, 0xC0, 22, 4, 0, rxBuffer, 8, usbTimeout);
		break;
		case 256:
			usb_control_msg(littleWirePointer, 0xC0, 22, 3, 0, rxBuffer, 8, usbTimeout);
		break;
		case 64:
			usb_control_msg(littleWirePointer, 0xC0, 22, 2, 0, rxBuffer, 8, usbTimeout);
		break;
		case 8:
			usb_control_msg(littleWirePointer, 0xC0, 22, 1, 0, rxBuffer, 8, usbTimeout);
		break;
		case 1:
			usb_control_msg(littleWirePointer, 0xC0, 22, 0, 0, rxBuffer, 8, usbTimeout);
		break;
	}
}
/*******************************************************************************/

/********************************************************************************
* Send one byte SPI message. Chip select is manual.
*    message: Message to send
*    Returns: Received SPI message
********************************************************************************/
unsigned char sendSpiMessage(usb_dev_handle* littleWirePointer,unsigned char message)
{
	usb_control_msg(littleWirePointer, 0xC0, 21, message, 0, rxBuffer, 8, usbTimeout);
	return rxBuffer[0];
}
/*******************************************************************************/
