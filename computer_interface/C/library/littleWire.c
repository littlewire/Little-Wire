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

#include "littleWire.h"


/********************************************************************************
* Try to connect to the device
*     Returns: lwHandle for success, 0 for fail
********************************************************************************/
littleWire* littleWire_connect()
{
	littleWire  *tempHandle = NULL;

	usb_init();
	usbOpenDevice(&tempHandle, VENDOR_ID, "*", PRODUCT_ID, "*", "*", NULL, NULL );

	return tempHandle;
}
/*******************************************************************************/

/********************************************************************************
* Set a GPIO pin High/Low
*     pin: Pin number
*     state: 1 for High, 0 for Low
********************************************************************************/
void digitalWrite(littleWire* lwHandle, unsigned char pin, unsigned char state)
{
	if(state){
		usb_control_msg(lwHandle, 0xC0, 18, pin, 0, rxBuffer, 8, USB_TIMEOUT);
	} else{
		usb_control_msg(lwHandle, 0xC0, 19, pin, 0, rxBuffer, 8, USB_TIMEOUT);
	}
}
/*******************************************************************************/

/********************************************************************************
* Set a GPIO pin input/output
*     pin: Pin number
*     mode: 1 for input, 0 for output
********************************************************************************/
void pinMode(littleWire* lwHandle, unsigned char pin, unsigned char mode)
{
	if(mode){
		usb_control_msg(lwHandle, 0xC0, 13, pin, 0, rxBuffer, 8, USB_TIMEOUT);
	} else {
		usb_control_msg(lwHandle, 0xC0, 14, pin, 0, rxBuffer, 8, USB_TIMEOUT);
	}
}
/*******************************************************************************/

/********************************************************************************
* Read a state of a GPIO pin
*     pin: Pin number
*     Returns: 1 for HIGH, 0 for LOW
********************************************************************************/
unsigned char digitalRead(littleWire* lwHandle, unsigned char pin)
{
	usb_control_msg(lwHandle, 0xC0, 21, pin, 0, rxBuffer, 8, USB_TIMEOUT);

	return rxBuffer[0];
}
/*******************************************************************************/

/********************************************************************************
* Read analog voltage from a spesific channel
*     channel: 0 for RESET pin, 1 for SCK pin, 2 for internal Temperature sensor
*     Returns: Analog voltage in 10bit resoultion
********************************************************************************/
unsigned int analogRead(littleWire* lwHandle, unsigned char channel)
{
	usb_control_msg(lwHandle, 0xC0, 15, channel, 0, rxBuffer, 8, USB_TIMEOUT);

	return ((rxBuffer[1] <<8) + (rxBuffer[0]));
}
/*******************************************************************************/

/********************************************************************************
* Initialize the Pwm module on the device
********************************************************************************/
void initPwm(littleWire* lwHandle)
{
	usb_control_msg(lwHandle, 0xC0, 16, 0, 0, rxBuffer, 8, USB_TIMEOUT);
}
/*******************************************************************************/

/********************************************************************************
* Update the compare values of Pwm outputs
*     channelA: Compare value of Channel A
*     channelB: Compare value of Channel B
/*******************************************************************************/
void updatePwmCompare(littleWire* lwHandle, unsigned char channelA, unsigned char channelB)
{
	usb_control_msg(lwHandle, 0xC0, 17, channelA, channelB, rxBuffer, 8, USB_TIMEOUT);
}
/*******************************************************************************/

/********************************************************************************
* Change the Pwm prescaler. Default: 1024
*     value: 1024/256/64/8/1
********************************************************************************/
void updatePwmPrescale(littleWire* lwHandle, unsigned int value)
{
	switch(value)
	{
		case 1024:
			usb_control_msg(lwHandle, 0xC0, 22, 4, 0, rxBuffer, 8, USB_TIMEOUT);
		break;
		case 256:
			usb_control_msg(lwHandle, 0xC0, 22, 3, 0, rxBuffer, 8, USB_TIMEOUT);
		break;
		case 64:
			usb_control_msg(lwHandle, 0xC0, 22, 2, 0, rxBuffer, 8, USB_TIMEOUT);
		break;
		case 8:
			usb_control_msg(lwHandle, 0xC0, 22, 1, 0, rxBuffer, 8, USB_TIMEOUT);
		break;
		case 1:
			usb_control_msg(lwHandle, 0xC0, 22, 0, 0, rxBuffer, 8, USB_TIMEOUT);
		break;
	}
}
/*******************************************************************************/

/********************************************************************************
* Send one byte SPI message. Chip select is manual.
*    message: Message to send
*    Returns: Received SPI message
********************************************************************************/
unsigned char sendSpiMessage(littleWire* lwHandle, unsigned char message)
{
	usb_control_msg(lwHandle, 0xC0, 21, message, 0, rxBuffer, 8, USB_TIMEOUT);

	return rxBuffer[0];
}
/*******************************************************************************/

