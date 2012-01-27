
/**
 * @author Ihsan Kehribar <ihsan@kehribar.me>
 * @author Omer Kilic <omerkilic@gmail.com>
 * @version 0.9
 *
 * @section LICENSE
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "littleWire.h"
#include <iostream>

//namespace LittleWire
//{

littleWire::littleWire()
{
	this->lwHandle = NULL;

	usb_init();

	usbOpenDevice(&this->lwHandle, VENDOR_ID, (char*)"*", PRODUCT_ID, (char*)"*", (char*)"*", NULL, NULL );

	// throw exception if handle == NULL?
}

littleWire::~littleWire()
{
	this->lwHandle = NULL;
}

void littleWire::digitalWrite(unsigned char pin, unsigned char state)
{
	if(state == OUTPUT){
		usb_control_msg(this->lwHandle, 0xC0, 18, pin, 0, rxBuffer, 8, USB_TIMEOUT);
	} else {
		usb_control_msg(this->lwHandle, 0xC0, 19, pin, 0, rxBuffer, 8, USB_TIMEOUT);
	}
}

void littleWire::pinMode(unsigned char pin, unsigned char mode)
{
	if(mode){
		usb_control_msg(this->lwHandle, 0xC0, 13, pin, 0, rxBuffer, 8, USB_TIMEOUT);
	} else {
		usb_control_msg(this->lwHandle, 0xC0, 14, pin, 0, rxBuffer, 8, USB_TIMEOUT);
	}
}

unsigned char littleWire::digitalRead(unsigned char pin)
{
	usb_control_msg(this->lwHandle, 0xC0, 21, pin, 0, rxBuffer, 8, USB_TIMEOUT);

	return rxBuffer[0];
}

unsigned int littleWire::analogRead(unsigned char channel)
{
	usb_control_msg(lwHandle, 0xC0, 15, channel, 0, rxBuffer, 8, USB_TIMEOUT);

	return ((rxBuffer[1] <<8) + (rxBuffer[0]));
}

void littleWire::pwm_init()
{
	usb_control_msg(this->lwHandle, 0xC0, 16, 0, 0, rxBuffer, 8, USB_TIMEOUT);
}

void littleWire::pwm_stop()
{
	usb_control_msg(this->lwHandle, 0xC0, 32, 0, 0, rxBuffer, 8, USB_TIMEOUT);
}

void littleWire::pwm_updateCompare(unsigned char channelA, unsigned char channelB)
{
	usb_control_msg(this->lwHandle, 0xC0, 17, channelA, channelB, rxBuffer, 8, USB_TIMEOUT);
}

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

void littleWire::spi_init()
{
	usb_control_msg(this->lwHandle, 0xC0, 23, 0, 0, rxBuffer, 8, USB_TIMEOUT);
}

unsigned char littleWire::spi_sendMessage(unsigned char message)
{
	usb_control_msg(this->lwHandle, 0xC0, 21, message, 0, rxBuffer, 8, USB_TIMEOUT);

	return rxBuffer[0];
}

void littleWire::spi_sendMessageMulti(unsigned char* sendBuffer, unsigned char* inputBuffer, unsigned char length, unsigned char mode)
{
	int i=0;
	if(length>4)
		length=4;
	usb_control_msg(this->lwHandle, 0xC0, (0xF0 + length + (mode<<3) ), (sendBuffer[1]<<8) + sendBuffer[0] , (sendBuffer[3]<<8) + sendBuffer[2], rxBuffer, 8, USB_TIMEOUT);
	for(i=0;i<length;i++)
		inputBuffer[i]=rxBuffer[i];
}

void littleWire::spi_updateDelay(unsigned int duration)
{
	usb_control_msg(this->lwHandle, 0xC0, 31, duration, 0, rxBuffer, 8, USB_TIMEOUT);
}

void littleWire::i2c_init()
{
	usb_control_msg(lwHandle, 0xC0, 24, 0, 0, rxBuffer, 8, USB_TIMEOUT);
}

void littleWire::i2c_beginTransmission(unsigned char address)
{
	usb_control_msg(this->lwHandle, 0xC0, 25, address, 0, rxBuffer, 8, USB_TIMEOUT);
}

void littleWire::i2c_send(unsigned char message)
{
	usb_control_msg(this->lwHandle, 0xC0, 26, message, 0, rxBuffer, 8, USB_TIMEOUT);
}

void littleWire::i2c_endTransmission()
{
	usb_control_msg(this->lwHandle, 0xC0, 27, 0, 0, rxBuffer, 8, USB_TIMEOUT);
}

void littleWire::i2c_requestFrom(unsigned char address,unsigned char numBytes,unsigned char* responseBuffer)
{
	int i,k;
	usb_control_msg(this->lwHandle, 0xC0, 30, address, numBytes, rxBuffer, 8, USB_TIMEOUT);
	
	for(i=0;i<numBytes;i++)
		responseBuffer[i]=rxBuffer[i];
}

void littleWire::servo_init()
{
	pwm_init(); // Initialize the PWM hardware.
	pinMode(PWMA, OUTPUT);
	pinMode(PWMB, OUTPUT); // Set PWM pins output.
	pwm_updatePrescaler(1024); // Make sure the PWM prescaler is set correctly.
}

void littleWire::servo_updateLocation(unsigned char locationChannelA, unsigned char locationChannelB)
{
	locationChannelA=(((locationChannelA/RANGE)*(MAX_LIMIT-MIN_LIMIT))+MIN_LIMIT)/STEP_SIZE;
	locationChannelB=(((locationChannelB/RANGE)*(MAX_LIMIT-MIN_LIMIT))+MIN_LIMIT)/STEP_SIZE;
	pwm_updateCompare(locationChannelA,locationChannelB);
}

//} //namespace

