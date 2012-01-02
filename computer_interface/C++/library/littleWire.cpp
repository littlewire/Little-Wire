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


#include "littleWire.h"

#include <iostream>

//namespace LittleWire
//{

/********************************************************************************
* Try to connect to the device
*     Returns: lwHandle for success, 0 for fail
********************************************************************************/
littleWire::littleWire()
{
	this->lwHandle = NULL;

	usb_init();

	usbOpenDevice(&this->lwHandle, VENDOR_ID, (char*)"*", PRODUCT_ID, (char*)"*", (char*)"*", NULL, NULL );

	// throw exception if handle == NULL?
}
/*******************************************************************************/

/********************************************************************************
* Set a GPIO pin High/Low
*     pin: Pin number
*     state: 1 for High, 0 for Low
********************************************************************************/
void littleWire::digitalWrite(unsigned char pin, unsigned char state)
{
	if(state == OUTPUT){
		usb_control_msg(this->lwHandle, 0xC0, 18, pin, 0, rxBuffer, 8, USB_TIMEOUT);
	} else {
		usb_control_msg(this->lwHandle, 0xC0, 19, pin, 0, rxBuffer, 8, USB_TIMEOUT);
	}
}
/*******************************************************************************/

/********************************************************************************
* Set a GPIO pin input/output
*     pin: Pin number
*     mode: 1 for input, 0 for output
********************************************************************************/
void littleWire::pinMode(unsigned char pin, unsigned char mode)
{
	if(mode){
		usb_control_msg(this->lwHandle, 0xC0, 13, pin, 0, rxBuffer, 8, USB_TIMEOUT);
	} else {
		usb_control_msg(this->lwHandle, 0xC0, 14, pin, 0, rxBuffer, 8, USB_TIMEOUT);
	}
}
/*******************************************************************************/

/********************************************************************************
* Read a state of a GPIO pin
*     pin: Pin number
*     Returns: 1 for HIGH, 0 for LOW
********************************************************************************/
unsigned char littleWire::digitalRead(unsigned char pin)
{
	usb_control_msg(this->lwHandle, 0xC0, 21, pin, 0, rxBuffer, 8, USB_TIMEOUT);

	return rxBuffer[0];
}
/*******************************************************************************/

/********************************************************************************
* Read analog voltage from a spesific channel
*     channel: 0 for RESET pin, 1 for SCK pin, 2 for internal Temperature sensor
*     Returns: Analog voltage in 10bit resoultion
********************************************************************************/
unsigned int littleWire::analogRead(unsigned char channel)
{
	usb_control_msg(lwHandle, 0xC0, 15, channel, 0, rxBuffer, 8, USB_TIMEOUT);

	return ((rxBuffer[1] <<8) + (rxBuffer[0]));
}
/*******************************************************************************/

/********************************************************************************
* Initialize the Pwm module on the device
********************************************************************************/
void littleWire::pwm_init()
{
	usb_control_msg(this->lwHandle, 0xC0, 16, 0, 0, rxBuffer, 8, USB_TIMEOUT);
}
/*******************************************************************************/

/********************************************************************************
* Stop the PWM module on the device
********************************************************************************/
void littleWire::pwm_stop()
{
	usb_control_msg(this->lwHandle, 0xC0, 32, 0, 0, rxBuffer, 8, USB_TIMEOUT);
}
/*******************************************************************************/

/********************************************************************************
* Update the compare values of Pwm outputs
*     channelA: Compare value of Channel A
*     channelB: Compare value of Channel B
/*******************************************************************************/
void littleWire::pwm_updateCompare(unsigned char channelA, unsigned char channelB)
{
	usb_control_msg(this->lwHandle, 0xC0, 17, channelA, channelB, rxBuffer, 8, USB_TIMEOUT);
}
/*******************************************************************************/

/********************************************************************************
* Change the Pwm prescaler. Default: 1024
*     value: 1024/256/64/8/1
********************************************************************************/
void littleWire::pwm_updatePrescaler(unsigned int value)
{
	switch(value)
	{
		case 1024:
			usb_control_msg(this->lwHandle, 0xC0, 22, 4, 0, rxBuffer, 8, USB_TIMEOUT);
		break;
		case 256:
			usb_control_msg(this->lwHandle, 0xC0, 22, 3, 0, rxBuffer, 8, USB_TIMEOUT);
		break;
		case 64:
			usb_control_msg(this->lwHandle, 0xC0, 22, 2, 0, rxBuffer, 8, USB_TIMEOUT);
		break;
		case 8:
			usb_control_msg(this->lwHandle, 0xC0, 22, 1, 0, rxBuffer, 8, USB_TIMEOUT);
		break;
		case 1:
			usb_control_msg(this->lwHandle, 0xC0, 22, 0, 0, rxBuffer, 8, USB_TIMEOUT);
		break;
	}
}
/*******************************************************************************/

/********************************************************************************
* Initialize SPI module
********************************************************************************/
void littleWire::spi_init()
{
	usb_control_msg(this->lwHandle, 0xC0, 23, 0, 0, rxBuffer, 8, USB_TIMEOUT);
}
/*******************************************************************************/


/********************************************************************************
* Send one byte SPI message. Chip select is manual.
*    message: Message to send
*    Returns: Received SPI message
********************************************************************************/
unsigned char littleWire::spi_sendMessage(unsigned char message)
{
	usb_control_msg(this->lwHandle, 0xC0, 21, message, 0, rxBuffer, 8, USB_TIMEOUT);

	return rxBuffer[0];
}
/*******************************************************************************/

/********************************************************************************
* Send multiple SPI messages. Chip select is manual.
*    sendBuffer: Message array to send
*    inputBuffer: Returned answer message
*	 length: Message length - maximum 4
*	 mode: 1 for auto chip select , 0 for manual
********************************************************************************/
void littleWire::spi_sendMessageMulti(unsigned char* sendBuffer, unsigned char* inputBuffer, unsigned char length, unsigned char mode)
{
	int i=0;
	if(length>4)
		length=4;
	usb_control_msg(this->lwHandle, 0xC0, (0xF0 + length + (mode<<3) ), (sendBuffer[1]<<8) + sendBuffer[0] , (sendBuffer[3]<<8) + sendBuffer[2], rxBuffer, 8, USB_TIMEOUT);
	for(i=0;i<length;i++)
		inputBuffer[i]=rxBuffer[i];
}
/*******************************************************************************/

/********************************************************************************
* Update SPI signal delay amount. Tune if neccessary to fit your requirements.
*	duration: Delay in microseconds.
********************************************************************************/
void littleWire::spi_updateDelay(unsigned int duration)
{
	usb_control_msg(this->lwHandle, 0xC0, 31, duration, 0, rxBuffer, 8, USB_TIMEOUT);
}
/*******************************************************************************/

/********************************************************************************
* Initialize i2c module on Little-Wire
********************************************************************************/
void littleWire::i2c_init()
{
	usb_control_msg(lwHandle, 0xC0, 24, 0, 0, rxBuffer, 8, USB_TIMEOUT);
}
/*******************************************************************************/

/********************************************************************************
* Start the i2c tranmission
*	address: Slave device address
********************************************************************************/
void littleWire::i2c_beginTransmission(unsigned char address)
{
	usb_control_msg(this->lwHandle, 0xC0, 25, address, 0, rxBuffer, 8, USB_TIMEOUT);
}
/*******************************************************************************/

/********************************************************************************
* Add new byte to the i2c send buffer
*	message: A byte to send.
********************************************************************************/
void littleWire::i2c_send(unsigned char message)
{
	usb_control_msg(this->lwHandle, 0xC0, 26, message, 0, rxBuffer, 8, USB_TIMEOUT);
}
/*******************************************************************************/

/********************************************************************************
* Send the whole message buffer to the slave at once and end the tranmssion.
********************************************************************************/
void littleWire::i2c_endTransmission()
{
	usb_control_msg(this->lwHandle, 0xC0, 27, 0, 0, rxBuffer, 8, USB_TIMEOUT);
}
/*******************************************************************************/

/********************************************************************************
* Request an reply / message from a slave device.
*	address: Slave address
*	numBytes: Number of bytes the slave will send.
*	responseBuffer: Array pointer which will hold the response from the slave
********************************************************************************/
void littleWire::i2c_requestFrom(unsigned char address,unsigned char numBytes,unsigned char* responseBuffer)
{
	int i,k;
	usb_control_msg(this->lwHandle, 0xC0, 30, address, numBytes, rxBuffer, 8, USB_TIMEOUT);
	
	for(k=0;k<8;k++)
		printf("%d-%d\n",k,rxBuffer[k]);
	
	for(i=0;i<numBytes;i++)
		responseBuffer[i]=rxBuffer[i];
}
/*******************************************************************************/
