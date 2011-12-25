/*


	ihsan Kehribar 
	November 2011

	
	http://kehibar.me/projects/Little-Wire



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
	usbOpenDevice(&tempHandle, vendorID, "*", productID, "*", "*", NULL, NULL );

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
		usb_control_msg(lwHandle, 0xC0, 18, pin, 0, rxBuffer, 8, usbTimeout);
	} else{
		usb_control_msg(lwHandle, 0xC0, 19, pin, 0, rxBuffer, 8, usbTimeout);
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
		usb_control_msg(lwHandle, 0xC0, 13, pin, 0, rxBuffer, 8, usbTimeout);
	} else {
		usb_control_msg(lwHandle, 0xC0, 14, pin, 0, rxBuffer, 8, usbTimeout);
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
	usb_control_msg(lwHandle, 0xC0, 21, pin, 0, rxBuffer, 8, usbTimeout);

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
	usb_control_msg(lwHandle, 0xC0, 15, channel, 0, rxBuffer, 8, usbTimeout);

	return ((rxBuffer[1] <<8) + (rxBuffer[0]));
}
/*******************************************************************************/

/********************************************************************************
* Initialize the Pwm module on the device
********************************************************************************/
void initPwm(littleWire* lwHandle)
{
	usb_control_msg(lwHandle, 0xC0, 16, 0, 0, rxBuffer, 8, usbTimeout);
}
/*******************************************************************************/

/********************************************************************************
* Update the compare values of Pwm outputs
*     channelA: Compare value of Channel A
*     channelB: Compare value of Channel B
/*******************************************************************************/
void updatePwmCompare(littleWire* lwHandle, unsigned char channelA, unsigned char channelB)
{
	usb_control_msg(lwHandle, 0xC0, 17, channelA, channelB, rxBuffer, 8, usbTimeout);
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
			usb_control_msg(lwHandle, 0xC0, 22, 4, 0, rxBuffer, 8, usbTimeout);
		break;
		case 256:
			usb_control_msg(lwHandle, 0xC0, 22, 3, 0, rxBuffer, 8, usbTimeout);
		break;
		case 64:
			usb_control_msg(lwHandle, 0xC0, 22, 2, 0, rxBuffer, 8, usbTimeout);
		break;
		case 8:
			usb_control_msg(lwHandle, 0xC0, 22, 1, 0, rxBuffer, 8, usbTimeout);
		break;
		case 1:
			usb_control_msg(lwHandle, 0xC0, 22, 0, 0, rxBuffer, 8, usbTimeout);
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
	usb_control_msg(lwHandle, 0xC0, 21, message, 0, rxBuffer, 8, usbTimeout);

	return rxBuffer[0];
}
/*******************************************************************************/

