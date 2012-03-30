/**
 * LittleWire
 * Processing interface for Little Wire hardware
 * http://kehribar.me/projects/Little-Wire/processing
 *
 * Copyright (C) 2011 ihsan Kehribar http://kehribar.me
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA  02111-1307  USA
 * 
 * @author      ihsan Kehribar http://kehribar.me
 * @modified    03/30/2012
 * @version     1 (1)
 */

package cc.littleWire;

import ch.ntb.usb.*;

/********************************************************************************
* Processing interface library for Little Wire
* @example Analog_Plot 
********************************************************************************/
public class LittleWire {
	
	public final static String VERSION = "1";
	private static Device myLittleWire;
	private boolean deviceConnected=false;
	private byte[] receiveBuffer=new byte[8];
	
	// General Purpose Pins
	public int PIN1 =	1;
	public int PIN2 =	2;
	public int PIN3 =	5;
	public int PIN4 =	0;
	
	// ADC Channels
	public int ADC_PIN3 =	0;
	public int ADC_PIN2	=	1;
	public int ADC_TEMP_SENS =	2;

	// PWM Pins
	public int PWM1	= PIN4;
	public int PWM2	= PIN1;

	//Usefull constants	
	public int INPUT = 1;
	public int OUTPUT = 0;
	public int LOW = 0;
	public int HIGH = 1;
	public int MANUAL_CS = 0;
	public int AUTO_CS = 1;
	
	double MIN_LIMIT = 0.45; // in miliseconds
	double MAX_LIMIT = 2.45; // in miliseconds
	double STEP_SIZE = 0.062; // in miliseconds
	double RANGE = 180.0; // in degrees
	
	public LittleWire() 
	{
		welcome();
	}
	
	/********************************************************************************
	* Try to connect to the device
	*     @return true for success, false for fail
	********************************************************************************/
	public boolean Connect()
	{
		  LibusbJava.usb_init(); // required inits for libusb-win32
		  LibusbJava.usb_find_busses(); //
		  LibusbJava.usb_find_devices(); //
	
		  // Find a specific USB device on the bus (by its VID/PID) and return a handle to the device
		  // This diverges a bit from native libusb-win32 code in that you don't have to loop through all the devices/busses searching for it.
		  myLittleWire = USB.getDevice((short) 0x1781, (short) 0x0c9f); // VID/PID combination of Little Wire
		  try
		  {
			myLittleWire.open(1, 0, -1);	  
		  }
		  catch(USBException e)
		  {
			  System.out.println("Little-Wire couldn't be found!");
			  deviceConnected=false;
			  return false;
		  }
		  deviceConnected=true;
		  System.out.println("Little-Wire connected!");
		  return true;
	}
	
	private void welcome() 
	{
		System.out.println("LittleWire by ihsan Kehribar http://kehribar.me");
	}
	
	public static String version() {
		return VERSION;
	}

	/********************************************************************************
	* Set a GPIO pin High/Low
	*     @param pin: Pin number
	*     @param state: 1 for High, 0 for Low
	********************************************************************************/
	public void digitalWrite(int pin, int state)
	{
		if(deviceConnected)
		{
			if(state==1) // set pin high
			{
				try {
					myLittleWire.controlMsg(0xC0, 18, pin,0,receiveBuffer,8,1000,false);
				} catch (USBException e) {
					e.printStackTrace();
				}
			}
			else // set pin low
			{
				try {
					myLittleWire.controlMsg(0xC0, 19, pin,0,receiveBuffer,8,1000,false);
				} catch (USBException e) {
					e.printStackTrace();
				}
			}
		}
		else
		{
			System.out.println("First, connect to Little-Wire!");
		}
	}
	/*******************************************************************************/

	/********************************************************************************
	* Set a GPIO pin input/output
	*     @param pin: Pin number
	*     @param mod: 1 for input, 0 for output
	********************************************************************************/
	public void pinMode(int pin, int mod)
	{
		if(deviceConnected)
		{
			if(mod==1) // set pin input
			{
				try {
					myLittleWire.controlMsg(0xC0, 13, pin,0,receiveBuffer,8,1000,false);
				} catch (USBException e) {
					e.printStackTrace();
				}
			}
			else // set pin low
			{
				try {
					myLittleWire.controlMsg(0xC0, 14, pin,0,receiveBuffer,8,1000,false);
				} catch (USBException e) {
					e.printStackTrace();
				}
			}
		}
		else
		{
			System.out.println("First, connect to Little-Wire!");
		}
	}
	/*******************************************************************************/

	/********************************************************************************
	* Read a state of a GPIO pin
	*     @param pin: Pin number
	*     @return 1 for HIGH, 0 for LOW
	********************************************************************************/
	public byte digitalRead(int pin)
	{
		if(deviceConnected)
		{
			try {
				myLittleWire.controlMsg(0xC0, 20, pin,0,receiveBuffer,8,1000,false);
				return receiveBuffer[0];
			} catch (USBException e) {
				e.printStackTrace();
				return -1;
			}
		}
		else
		{
			System.out.println("First, connect to Little-Wire!");
			return -1;
		}
	}
	/*******************************************************************************/

	/********************************************************************************
	* Read analog voltage from a spesific channel
	*     @param channel: 0 for RESET pin, 1 for SCK pin, 2 for internal Temperature sensor
	*     @return Analog voltage in 10bit resoultion
	********************************************************************************/
	public int analogRead(int channel)
	{
		if(deviceConnected)
		{
			int low,high;
			try {
				myLittleWire.controlMsg(0xC0, 15, channel,0,receiveBuffer,8,1000,false);
				// Manual signed to unsigned conversion ... Shame on you Java ...
				if(receiveBuffer[0]<0)
					low=receiveBuffer[0]+256;
				else
					low=receiveBuffer[0];
				if(receiveBuffer[1]<0)
					high=receiveBuffer[1]+256;
				else
					high=receiveBuffer[1];
				return (high*256)+low;
			} catch (USBException e) {
				e.printStackTrace();
				return -1;
			}
		}
		else
		{
			System.out.println("First, connect to Little-Wire!");
			return -1;
		}
	}
	/*******************************************************************************/

	/********************************************************************************
	* Initialize the Pwm module on the device
	********************************************************************************/
	public void pwm_init()
	{
		if(deviceConnected)
		{
			try {
				myLittleWire.controlMsg(0xC0, 16, 0,0,receiveBuffer,8,1000,false);				
			} catch (USBException e) {
				e.printStackTrace();			
			}
		}
		else
		{
			System.out.println("First, connect to Little-Wire!");			
		}
	}
	/*******************************************************************************/
	
	/********************************************************************************
	* Stop the Pwm module on the device
	********************************************************************************/
	public void pwm_stop()
	{
		if(deviceConnected)
		{
			try {
				myLittleWire.controlMsg(0xC0, 32, 0,0,receiveBuffer,8,1000,false);				
			} catch (USBException e) {
				e.printStackTrace();			
			}
		}
		else
		{
			System.out.println("First, connect to Little-Wire!");			
		}
	}
	/*******************************************************************************/	

	/********************************************************************************
	* Update the compare values of Pwm outputs
	*     @param channelA: Compare value of Channel A
	*     @param channelB: Compare value of Channel B
	/*******************************************************************************/
	public void pwm_updateCompare(int channelA, int channelB)
	{
		if(deviceConnected)
		{
			try {
				myLittleWire.controlMsg(0xC0, 17, channelA,channelB,receiveBuffer,8,1000,false);
			} catch (USBException e) {
				e.printStackTrace();
			}
		}
		else
		{
			System.out.println("First, connect to Little-Wire!");
		}
	}
	/*******************************************************************************/

	/********************************************************************************
	* Change the Pwm prescaler. Default: 1024
	*     @param value: 1024/256/64/8/1
	********************************************************************************/
	public void pwm_updatePrescaler(int value)
	{
		if(deviceConnected)
		{
	        if (value == 1)
	        {
				try {
					myLittleWire.controlMsg(0xC0, 22, 0,0,receiveBuffer,8,1000,false);
				} catch (USBException e) {
					e.printStackTrace();
				}
	        }
			else if (value == 8)
			{
				try {
					myLittleWire.controlMsg(0xC0, 22, 1,0,receiveBuffer,8,1000,false);
				} catch (USBException e) {
					e.printStackTrace();
				}
			}
			else if (value == 64)
			{
				try {
					myLittleWire.controlMsg(0xC0, 22, 2,0,receiveBuffer,8,1000,false);
				} catch (USBException e) {
					e.printStackTrace();
				}
			}
			else if (value == 256)
			{
				try {
					myLittleWire.controlMsg(0xC0, 22, 3,0,receiveBuffer,8,1000,false);
				} catch (USBException e) {
					e.printStackTrace();
				}
			}
			else if (value == 1024)
			{
				try {
					myLittleWire.controlMsg(0xC0, 22, 4,0,receiveBuffer,8,1000,false);
				} catch (USBException e) {
					e.printStackTrace();
				}
			}
		}
		else
		{
			System.out.println("First, connect to Little-Wire!");
		}
	}
	/*******************************************************************************/

	// /********************************************************************************
	// * Initialize SPI module
	// ********************************************************************************/
	// void spi_init()
	// {
		// if(deviceConnected)
		// {
			// try {
				// myLittleWire.controlMsg(0xC0, 23, 0,0,receiveBuffer,8,1000,false);
			// } catch (USBException e) {
				// e.printStackTrace();
			// }
		// }
		// else
		// {
			// System.out.println("First, connect to Little-Wire!");
		// }
	// }
	// /*******************************************************************************/

	// /********************************************************************************
	// * Send one byte SPI message. Chip select is manual.
	// *    message: Message to send
	// *    Returns: Received SPI message
	// ********************************************************************************/
	// int spi_sendMessage(int message)
	// {
		// if(deviceConnected)
		// {
			// try {
				// myLittleWire.controlMsg(0xC0, 21, message,0,receiveBuffer,8,1000,false);
				// return receiveBuffer[0];
			// } catch (USBException e) {
				// e.printStackTrace();
			// }
		// }
		// else
		// {
			// System.out.println("First, connect to Little-Wire!");
		// }
	// }
	// /*******************************************************************************/

	// /********************************************************************************
	// * Send multiple SPI messages. Chip select is manual/automatic.
	// *    sendBuffer: Message array to send
	// *    inputBuffer: Returned answer message
	// *	 length: Message length - maximum 4
	// *	 mode: 1 for auto chip select , 0 for manual
	// ********************************************************************************/
	// public void spi_sendMessage_multiple(littleWire* lwHandle, unsigned char * sendBuffer, unsigned char * inputBuffer, unsigned char length ,unsigned char mode);
	// /*******************************************************************************/

	// /********************************************************************************
	// * Update SPI signal delay amount. Tune if neccessary to fit your requirements.
	// *	duration: Delay amount.
	// ********************************************************************************/
	// public void spi_updateDelay(littleWire* lwHandle, unsigned int duration);
	// /*******************************************************************************/

	// /********************************************************************************
	// * Initialize i2c module on Little-Wire
	// ********************************************************************************/
	// public void i2c_init(littleWire* lwHandle);
	// /*******************************************************************************/

	// /********************************************************************************
	// * Start the i2c tranmission
	// *	address: Slave device address
	// ********************************************************************************/
	// public void i2c_beginTransmission(littleWire* lwHandle, unsigned char address);
	// /*******************************************************************************/

	// /********************************************************************************
	// * Add new byte to the i2c send buffer
	// *	message: A byte to send.
	// ********************************************************************************/
	// public void i2c_send(littleWire* lwHandle,unsigned char message);
	// /*******************************************************************************/

	// /********************************************************************************
	// * Send the whole message buffer to the slave at once and end the tranmssion.
	// ********************************************************************************/
	// public void i2c_endTransmission(littleWire* lwHandle);
	// /*******************************************************************************/

	// /********************************************************************************
	// * Request a reply / message from a slave device.
	// *	address: Slave address
	// *	numBytes: Number of bytes the slave will send.
	// *	responseBuffer: Array pointer which will hold the response from the slave
	// ********************************************************************************/
	// public void i2c_requestFrom(littleWire* lwHandle,unsigned char address,unsigned char numBytes,unsigned char * responseBuffer);
	// /*******************************************************************************/

	/********************************************************************************
	* Servo module initialization
	********************************************************************************/
	public void servo_init()
	{
		this.pwm_init();
		this.pinMode(PWM1, OUTPUT);
		this.pinMode(PWM2, OUTPUT);
		this.pwm_updatePrescaler(1024);
	}
	/*******************************************************************************/

	/********************************************************************************
	* Servo locations update
	*	@param locationChannelA in degrees
	*	@param locationChannelB in degrees
	********************************************************************************/
	public void servo_updateLocation(int locationChannelA,int locationChannelB)
	{
		locationChannelA=(int)((((locationChannelA/RANGE)*(MAX_LIMIT-MIN_LIMIT))+MIN_LIMIT)/STEP_SIZE);
		locationChannelB=(int)((((locationChannelB/RANGE)*(MAX_LIMIT-MIN_LIMIT))+MIN_LIMIT)/STEP_SIZE);
		this.pwm_updateCompare(locationChannelA,locationChannelB);
	}
	/*******************************************************************************/	
	
}

