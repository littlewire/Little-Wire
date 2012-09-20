/**
 * @author Ihsan Kehribar <ihsan@kehribar.me>
 * @version 1.1
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
 
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibUsbDotNet;
using LibUsbDotNet.Main;
using System.Threading;

namespace  littleWireLib
{
    public class littleWire
    {
        public bool lwStatus;

        #region onewire definitions
        byte	crc8;
        byte[]	ROM_NO = new byte[8];
        int		LastDiscrepancy;
        int		LastFamilyDiscrepancy;
        int		LastDeviceFlag;
        /******************************************************************************
        / Taken from: http://www.maxim-ic.com/appnotes.cfm/appnote_number/187
        /*****************************************************************************/
        static byte[] dscrc_table = {
            0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
          157,195, 33,127,252,162, 64, 30, 95,  1,227,189, 62, 96,130,220,
           35,125,159,193, 66, 28,254,160,225,191, 93,  3,128,222, 60, 98,
          190,224,  2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
           70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89,  7,
          219,133,103, 57,186,228,  6, 88, 25, 71,165,251,120, 38,196,154,
          101, 59,217,135,  4, 90,184,230,167,249, 27, 69,198,152,122, 36,
          248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91,  5,231,185,
          140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
           17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
          175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
           50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
          202,148,118, 40,171,245, 23, 73,  8, 86,180,234,105, 55,213,139,
           87,  9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
          233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
          116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53};
        /*****************************************************************************/
        #endregion
	
		#region definitions and etc.
	
        // Pin mappings
        public static byte PIN1 = 1;
        public static byte PIN2 = 2;
        public static byte PIN3 = 5;
        public static byte PIN4 = 0;

        // Pin directions
        public static byte IN = 1;
        public static byte OUT = 0;
		
		// ADC voltage references
		public static byte VREF_VCC = 0;
		public static byte VREF_1100mV = 1;
		public static byte VREF_2560mV = 2;
		
		// I2C defintions
		public static byte END_WITH_STOP = 1;
		public static byte NO_STOP = 0;
		public static byte READ = 1;
		public static byte WRITE = 0;

        // Pin directions (aliases)
        public static byte INPUT = IN;
        public static byte OUTPUT = OUT;

        // Generic states
		public static byte DISABLE = 0;
		public static byte ENABLE = 1;    
        public static byte HIGH = 1;
        public static byte LOW = 0;

		// Spi constants
		public static byte AUTO_CS = 1;
		public static byte MANUAL_CS = 0;
		
		// Servo constants
		private static double STEP_SIZE = 0.062; // in miliseconds
		private static double MIN_LIMIT = 0.8; // in miliseconds
		private static double MAX_LIMIT = 2.2; // in miliseconds
		private static double RANGE = 180; // in degrees
		
        private static UsbDeviceFinder MyUsbFinder;
        private static UsbDevice MyUsbDevice;
        private UsbSetupPacket MySetupPacket;
		
		#endregion

		#region low level stuff
		
        /// <summary>
        /// Simple constructor
        /// </summary>
        public littleWire()
        {
            MyUsbFinder = new UsbDeviceFinder(0x1781, 0x0C9F);
        }

        /// <summary>
        /// Check the connection
        /// </summary>
        /// <returns>True if still alive</returns>
        public bool isAlive()
        {
            return lwStatus;
        }

        /// <summary>
        /// Try to connect to the device
        /// </summary>
        /// <returns>1 for success, 0 for fail</returns>
        public int connect()
        {
            try
            {
                // Find and open the usb device.
                MyUsbDevice = UsbDevice.OpenUsbDevice(MyUsbFinder);

                // If the device is open and ready
                if (MyUsbDevice == null) throw new Exception("Device Not Found.");

                // If this is a "whole" usb device (libusb-win32, linux libusb)
                // it will have an IUsbDevice interface. If not (WinUSB) the 
                // variable will be null indicating this is an interface of a 
                // device.
                IUsbDevice wholeUsbDevice = MyUsbDevice as IUsbDevice;
                if (!ReferenceEquals(wholeUsbDevice, null))
                {
                    // This is a "whole" USB device. Before it can be used, 
                    // the desired configuration and interface must be selected.

                    // Select config #1
                    wholeUsbDevice.SetConfiguration(1);

                    // Claim interface #0.
                    wholeUsbDevice.ClaimInterface(0);
                }

                return 1; // Device found!
            }
            catch
            {
                return 0; // Failed to find one.
            }
        }
		
		/// <summary>
		/// Reads the firmware version of the Little Wire
		/// </summary>
		/// <returns> Firmware version. 0xXY => X: Primary version	Y: Minor version </returns>
		public byte readFirmwareVersion()
		{
			byte[] buffer_ = new byte[8];
			int whatIsThis = 8;
			MySetupPacket = new UsbSetupPacket(0xC0, 34, 0, 0, 8);
			lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);			
			return buffer_[0];
		}
		
		#endregion

		#region gpio related
		
        /// <summary>
        /// Set a GPIO pin High/Low
        /// </summary>
        /// <param name="pin">Pin number</param>
        /// <param name="state">HIGH or LOW</param>
        public void digitalWrite(byte pin, byte state)
        { 
            if(state==1)
                setPinHigh(pin);
            else if(state==0)
                setPinLow(pin);
        }

		/// <summary>
		/// Set a GPIO pin high
		/// </summary>
		/// <param name="pin">Pin number</param>
		public void setPinHigh(byte pin)
		{
			byte[] buffer_ = new byte[8];
			int whatIsThis = 8;
			MySetupPacket = new UsbSetupPacket(0xC0, 18, pin, 0, 8);
			lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
		}
		/// <summary>
		/// Set a GPIO pin low
		/// </summary>
		/// <param name="pin">Pin number</param>
		public void setPinLow(byte pin)
		{
			byte[] buffer_ = new byte[8];
			int whatIsThis = 8;
			MySetupPacket = new UsbSetupPacket(0xC0, 19, pin, 0, 8);
			lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
		}

        /// <summary>
        /// Set a GPIO pin input/output
        /// </summary>
        /// <param name="pin">Pin number</param>
        /// <param name="mod">1 for input, 0 for output</param>
        public void pinMode(byte pin, byte mod)
        {
            if (mod == 1)
                setPinInput(pin);
            else if (mod == 0)
                setPinOutput(pin);
        }
		
		/// <summary>
		/// Set a GPIO pin input
		/// </summary>
		/// <param name="pin">Pin number</param>
		private void setPinInput(byte pin)
		{
			byte[] buffer_ = new byte[8];
			int whatIsThis = 8;
			MySetupPacket = new UsbSetupPacket(0xC0, 13, pin, 0, 8);
			lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
		}
		
		/// <summary>
		/// Set a GPIO pin output
		/// </summary>
		/// <param name="pin">Pin number</param>
		private void setPinOutput(byte pin)
		{
			byte[] buffer_ = new byte[8];
			int whatIsThis = 8;
			MySetupPacket = new UsbSetupPacket(0xC0, 14, pin, 0, 8);
			lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
		}
        
        /// <summary>
        /// Read the state of a GPIO pin
        /// </summary>
        /// <param name="pin">Pin number</param>
        /// <returns>1 for HIGH, 0 for LOW</returns>
        public byte digitalRead(byte pin)
        {
            byte[] buffer_ = new byte[8];
            int whatIsThis = 8;
            MySetupPacket = new UsbSetupPacket(0xC0, 20, pin, 0, 8);
            lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
            return buffer_[0];
        }
		
		/// <summary>
		/// Sets the state of the internal pullup resistor.
		/// </summary>
		/// <param name="pin">Pin number</param>
		/// <param name="state">ENABLE or DISABLE</param>
		private void internalPullup(byte pin, byte state)
		{
			byte[] buffer_ = new byte[8];
			int whatIsThis = 8;
			if(state==ENABLE)
				MySetupPacket = new UsbSetupPacket(0xC0, 18, pin, 0, 8);
			else
				MySetupPacket = new UsbSetupPacket(0xC0, 19, pin, 0, 8);
			lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
		}
		
		#endregion
		
		#region adc related
        		
		/// <summary>
		/// Initialize the analog module. VREF_VCC is the standard voltage coming from the USB plug. Others are the Attiny's internal voltage references.
		/// </summary>
		/// <param name="voltageRef"> VREF_VCC, VREF_2560mV or VREF_1100mV </param>
		public void analog_init(byte voltageRef)
		{
			byte[] buffer_ = new byte[8];
            int whatIsThis = 8;
            MySetupPacket = new UsbSetupPacket(0xC0, 35, (short)((byte)(voltageRef<<8) + 0x07), 0, 8);
            lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);          
		}		

        /// <summary>
        /// Read analog voltage from a spesific channel
        /// </summary>
        /// <param name="channel">ADC_PIN2, ADC_PIN3 or ADC_TEMP_SENS</param>
        /// <returns>Analog voltage in 10bit resoultion</returns>
        public ushort analogRead(byte channel)
        {
            byte[] buffer_ = new byte[8];
            int whatIsThis = 8;
            MySetupPacket = new UsbSetupPacket(0xC0, 15, channel, 0, 8);
            lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
            return (ushort)((buffer_[1] <<8) + (buffer_[0]));
        }
		
		#endregion
        
		#region pwm related
		
        /// <summary>
        /// Initialize the Pwm module on the device
        /// </summary>
        public void pwm_init()
        {
            byte[] buffer_ = new byte[8];
            int whatIsThis = 8;
            MySetupPacket = new UsbSetupPacket(0xC0, 16, 0, 0, 8);
            lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
        }
		
		/// <summary>
        /// Initialize the Pwm module on the device
        /// </summary>
        public void pwm_stop()
        {
            byte[] buffer_ = new byte[8];
            int whatIsThis = 8;
            MySetupPacket = new UsbSetupPacket(0xC0, 32, 0, 0, 8);
            lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
        }
		
		/// <summary>
		/// Update the compare values of Pwm outputs
		/// </summary>
		/// <param name="channelA">Channel A</param>
		/// <param name="channelB">Channel B</param>
		public void pwm_updateCompare(byte channelA, byte channelB)
		{
			byte[] buffer_ = new byte[8];
			int whatIsThis = 8;
			MySetupPacket = new UsbSetupPacket(0xC0, 17, channelA, channelB, 8);
			lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
		}
		
		/// <summary>
		/// Change the Pwm prescaler. Default: 1024
		/// </summary>
		/// <param name="value">1024/256/64/8/1</param>
		public void pwm_updatePrescaler(UInt16 value)
		{
			byte[] buffer_ = new byte[8];
			int whatIsThis = 8;
			if (value == 1)
				MySetupPacket = new UsbSetupPacket(0xC0, 22, 0, 0, 8);
			else if (value == 8)
				MySetupPacket = new UsbSetupPacket(0xC0, 22, 1, 0, 8);
			else if (value == 64)
				MySetupPacket = new UsbSetupPacket(0xC0, 22, 2, 0, 8);
			else if (value == 256)
				MySetupPacket = new UsbSetupPacket(0xC0, 22, 3, 0, 8); 
			else if (value == 1024)
				MySetupPacket = new UsbSetupPacket(0xC0, 22, 4, 0, 8);
			lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
		}
		
		#endregion
		
		#region spi related
		
		/// <summary>
		/// Initialize the SPI module
		/// </summary>
		public void spi_init()
		{
			byte[] buffer_ = new byte[8];
			int whatIsThis = 8;
			MySetupPacket = new UsbSetupPacket(0xC0, 23, 0, 0, 8);
			lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
		}

        /// <summary>
        /// Send SPI message(s). SPI Mode is 0.
        /// </summary>
        /// <param name="sendBuffer">Message array to send</param>
		/// <param name="inputBuffer">Returned array message</param>
		/// <param name="length">Mesage length -> Max = 4</param>
		/// <param name="mode">AUTO_CS or MANUAL_CS</param>
        public void spi_sendMessage(byte[] sendBuffer, byte[] inputBuffer, byte length, byte mode)
        {
            byte[] buffer_ = new byte[8];
            int whatIsThis = 8;
            MySetupPacket = new UsbSetupPacket(0xC0, (byte)(0xF0 + length + (byte)(mode << 3)), (short)((sendBuffer[1] << 8) + sendBuffer[0]), (short)((sendBuffer[3] << 8) + sendBuffer[2]), 8);
            lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
			
			MySetupPacket = new UsbSetupPacket(0xC0, 40, 0, 0, 8);
			lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
			for(int i=0;i<length;i++)
				inputBuffer[i]=buffer_[i];
        }
		
		/// <summary>
		/// Send one byte SPI message over MOSI pin. Slightly slower than the actual one. There isn't any chip select control involved. Useful for debug console.
		/// </summary>
		/// <param name="message">Message to send</param>
		public byte debugSpi(byte message)
		{
			byte[] buffer_ = new byte[8];
			int whatIsThis = 8;
			MySetupPacket = new UsbSetupPacket(0xC0, 33, message, 0, 8);
			lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
			
			MySetupPacket = new UsbSetupPacket(0xC0, 40, 0, 0, 8);
			lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
			return buffer_[0];
		}
		
		/// <summary>
		/// Change the SPI message frequency by adjusting delay duration. By default, Little-Wire sends the SPI messages with maximum speed. 
		/// If your hardware can't catch up with the speed, increase the duration value to lower the SPI speed.
		/// </summary>
		/// <param name="duration">Amount of delay</param>
		public void spi_updateDelay(byte duration)
		{
			byte[] buffer_ = new byte[8];
			int whatIsThis = 8;
			MySetupPacket = new UsbSetupPacket(0xC0, 31, duration, 0, 8);
			lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
		}
		
		#endregion
		
		#region i2c related
				
		/// <summary>
		/// Initialize the I2C module on the Little-Wire 
		/// </summary>
		public void i2c_init()
		{
			byte[] buffer_ = new byte[8];
			int whatIsThis = 8;
			MySetupPacket = new UsbSetupPacket(0xC0, 44, 0, 0, 8);
			lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
		}		
		
		/// <summary>
		/// Start the i2c communication
		/// </summary>
		/// <param name="address7bit">7 bit slave address</param>
		/// <param name="direction">READ or WRITE</param>
		/// <returns>1 if received ACK</returns>
		public byte i2c_start(byte address7bit, byte direction)
		{
			byte[] buffer_ = new byte[8];
			int whatIsThis = 8;
			MySetupPacket = new UsbSetupPacket(0xC0, 45, (short)((address7bit<<1)|direction), 0, 8);
			lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
						
			MySetupPacket = new UsbSetupPacket(0xC0, 40, 0, 0, 8);
			lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
			return buffer_[0];
		}
		
		/// <summary>
		/// Send byte(s) over i2c bus
		/// </summary>
		/// <param name="sendBuffer">Message array to send</param>
		/// <param name="length">Message length -> Max = 4</param>
		/// <param name="endWithStop">ENDWITH_STOP or NO_STOP -- Should we send a stop condition after this bufffer?</param>
		public void i2c_write(byte[] sendBuffer, byte length, byte endWithStop)
		{
			byte[] buffer_ = new byte[8];
			int whatIsThis = 8;
			MySetupPacket = new UsbSetupPacket(0xC0, (byte)(0xE0 + length + (endWithStop<<3) ), (short)((sendBuffer[1]<<8) + sendBuffer[0]) , (short)((sendBuffer[3]<<8) + sendBuffer[2]),8);
			lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
		}
		
		/// <summary>
		/// Read byte(s) over i2c bus
		/// </summary>
		/// <param name="readBuffer">Returned message array</param>
		/// <param name="length">Message length -> Max = 8</param>
		/// <param name="endWithStop">ENDWITH_STOP or NO_STOP -- Should we send a stop condition after this bufffer?</param>
		public void i2c_read(byte[] readBuffer, byte length, byte endWithStop)
		{
			byte[] buffer_ = new byte[8];
			int whatIsThis = 8;
			if(endWithStop!=0)
				MySetupPacket = new UsbSetupPacket(0xC0, 46,(short)((length<<8) + 0) , 0, 8);
			else
				MySetupPacket = new UsbSetupPacket(0xC0, 46,(short)((length<<8) + 1) , 1, 8);
			lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
						
			MySetupPacket = new UsbSetupPacket(0xC0, 40, 0, 0, 8);
			lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
			for(int i=0;i<length;i++)
				readBuffer[i]=buffer_[i];
		}
				
		/// <summary>
		/// Update i2c signal delay amount. Tune if neccessary to fit your requirements.
		/// </summary>
		/// <param name="duration">Amount of delay</param>
		public void i2c_updateDelay(byte duration)
		{
			byte[] buffer_ = new byte[8];
			int whatIsThis = 8;
			MySetupPacket = new UsbSetupPacket(0xC0, 49, duration, 0, 8);
			lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
		}
		
		#endregion
		
		#region onewire related

        /// <summary>
        /// Send a single bit over onewire bus.
        /// </summary>
        /// <param name="bitValue">1 or 0</param>
        public void onewire_sendBit(byte bitValue)
        {
            byte[] buffer_ = new byte[8];
            int whatIsThis = 8;
            MySetupPacket = new UsbSetupPacket(0xC0, 51, bitValue, 0, 8);
            lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
        }

        /// <summary>
        /// Send a byte over onewire bus
        /// </summary>
        /// <param name="messageToSend">Message to send</param>
        public void onewire_writeByte(byte messageToSend)
        {
            byte[] buffer_ = new byte[8];
            int whatIsThis = 8;
            MySetupPacket = new UsbSetupPacket(0xC0, 42, messageToSend, 0, 8);
            lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
            Thread.Sleep(3);
        }

        /// <summary>
        /// Reads a byte over onewire bus
        /// </summary>
        /// <returns>Returned byte</returns>
        public byte onewire_readByte()
        {
            byte[] buffer_ = new byte[8];
            int whatIsThis = 8;
            MySetupPacket = new UsbSetupPacket(0xC0, 43, 0, 0, 8);
            lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
            Thread.Sleep(3);
            MySetupPacket = new UsbSetupPacket(0xC0, 40, 0, 0, 8);
            lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
            return buffer_[0];
        }

        /// <summary>
        /// Reads a single bit over onewire bus
        /// </summary>
        /// <returns>Returned bit</returns>
        public byte onewire_readBit()
        {
            byte[] buffer_ = new byte[8];
            int whatIsThis = 8;
            MySetupPacket = new UsbSetupPacket(0xC0, 50, 0, 0, 8);
            lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
            MySetupPacket = new UsbSetupPacket(0xC0, 40, 0, 0, 8);
            lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
            return buffer_[0];
        }

        /// <summary>
        /// Send a reset pulse over onewire bus
        /// </summary>
        /// <returns>Nonzero if any device presents on the bus</returns>
        public byte onewire_resetPulse()
        {
            byte[] buffer_ = new byte[8];
            int whatIsThis = 8;
            MySetupPacket = new UsbSetupPacket(0xC0, 41, 0, 0, 8);
            lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
            MySetupPacket = new UsbSetupPacket(0xC0, 40, 0, 0, 8);
            lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
            return buffer_[0];
        }

        /// <summary>
        /// Start searching for device address on the onewire bus.
        /// </summary>
        /// <param name="address">Found address array</param>
        /// <returns>Nonzero if any device found</returns>
        public byte onewire_firstAddress(byte[] address)
        {
            LastDiscrepancy = 0;
            LastDeviceFlag = 0;
            LastFamilyDiscrepancy = 0;

            return onewire_nextAddress(address);
        }

        public byte docrc8(byte value)
        {
           // See Maxim Application Note 27

           crc8 = dscrc_table[crc8 ^ value];

           return crc8;
        }

        /// <summary>
        /// Try to find the next adress on the onewire bus.
        /// </summary>
        /// <param name="address">Found address array</param>
        /// <returns>Nonzero if any new device found</returns>
        public byte onewire_nextAddress(byte[] address)
        {
           byte id_bit_number;
           byte last_zero, rom_byte_number, search_result;
           byte id_bit, cmp_id_bit;
           byte rom_byte_mask, search_direction;
           byte qq;

           // initialize for search
           id_bit_number = 1;
           last_zero = 0;
           rom_byte_number = 0;
           rom_byte_mask = 1;
           search_result = 0;
           crc8 = 0;

           // if the last call was not the last one
           if (LastDeviceFlag==0)
           {
              // 1-Wire reset
              if (onewire_resetPulse()==0)
              {
                 // reset the search
                 LastDiscrepancy = 0;
                 LastDeviceFlag = 0;
                 LastFamilyDiscrepancy = 0;
                 return 0;
              }

              // issue the search command 
              onewire_writeByte(0xF0);  

              // loop to do the search
              do
              {
                 // read a bit and its complement
                 id_bit = onewire_readBit();
                 cmp_id_bit = onewire_readBit();

                 // check for no devices on 1-wire
                 if ((id_bit == 1) && (cmp_id_bit == 1))
                    break;
                 else
                 {
                    // all devices coupled have 0 or 1
                    if (id_bit != cmp_id_bit)
                       search_direction = id_bit;  // bit write value for search
                    else
                    {
                       // if this discrepancy if before the Last Discrepancy
                       // on a previous next then pick the same as last time
                        if (id_bit_number < LastDiscrepancy)
                        {
                            if (((ROM_NO[rom_byte_number] & rom_byte_mask) > 0))
                                search_direction = 1;
                            else
                                search_direction = 0;
                        }
                        else
                        {
                            // if equal to last pick 1, if not then pick 0
                            if (id_bit_number == LastDiscrepancy)
                                search_direction = 1;
                            else
                                search_direction = 0;
                        }

                       // if 0 was picked then record its position in LastZero
                       if (search_direction == 0)
                       {
                          last_zero = id_bit_number;

                          // check for Last discrepancy in family
                          if (last_zero < 9)
                             LastFamilyDiscrepancy = last_zero;
                       }
                    }

                    // set or clear the bit in the ROM byte rom_byte_number
                    // with mask rom_byte_mask
                    if (search_direction == 1)
                      ROM_NO[rom_byte_number] |= rom_byte_mask;
                    else
                      ROM_NO[rom_byte_number] &= (byte)~rom_byte_mask;

                    // serial number search direction write bit
                    onewire_sendBit(search_direction);

                    // increment the byte counter id_bit_number
                    // and shift the mask rom_byte_mask
                    id_bit_number++;
                    rom_byte_mask <<= 1;

                    // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
                    if (rom_byte_mask == 0)
                    {
                        docrc8(ROM_NO[rom_byte_number]);  // accumulate the CRC
                        rom_byte_number++;
                        rom_byte_mask = 1;
                    }
                 }
              }
              while(rom_byte_number < 8);  // loop until through all ROM bytes 0-7

              // if the search was successful then
              if (!((id_bit_number < 65) || (crc8 != 0)))
              {
                 // search successful so set LastDiscrepancy,LastDeviceFlag,search_result
                 LastDiscrepancy = last_zero;

                 // check for last device
                 if (LastDiscrepancy == 0)
                    LastDeviceFlag = 1;
         
                 search_result = 1;
              }
           }

           // if no device found then reset counters so next 'search' will be like a first
           if (search_result==0 || ROM_NO[0]==0)
           {
              LastDiscrepancy = 0;
              LastDeviceFlag = 0;
              LastFamilyDiscrepancy = 0;
              search_result = 0;
           }

           if(search_result!=0)
           {
		        for(qq=0;qq<8;qq++)
			        address[qq]=ROM_NO[qq];
           }
   
           return search_result;
        }

		#endregion
		
		#region softPWM related

        /// <summary>
        /// Sets the state of the Software PWM module on the Little Wire
        /// </summary>
        /// <param name="state">ENABLE or DISABLE</param>
        public void softPWM_state(byte state)
        {
            byte[] buffer_ = new byte[8];
            int whatIsThis = 8;
            MySetupPacket = new UsbSetupPacket(0xC0, 47, state, 0, 8);
            lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
        }

        /// <summary>
        /// Updates the values of softPWM modules
        /// </summary>
        /// <param name="ch1">Value of channel 1 - pin4</param>
        /// <param name="ch2">Value of channel 2 - pin1</param>
        /// <param name="ch3">Value of channel 3 - pin2</param>
        public void softPWM_write(byte ch1, byte ch2, byte ch3)
        {
            byte[] buffer_ = new byte[8];
            int whatIsThis = 8;
            MySetupPacket = new UsbSetupPacket(0xC0, 48, (short)((ch2 << 8) | ch1), ch3, 8);
            lwStatus = MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
        }

		#endregion

        #region servo related

        /// <summary>
        /// Initialize the PWM module on the Little-Wire with the Servo special settings.
        /// </summary>
        public void servo_init()
        {           
            pwm_init(); // Initialize the PWM hardware.
            pinMode(PIN4, OUTPUT); // Set PWM pins output.
            pinMode(PIN1, OUTPUT); // Set PWM pins output.
            pwm_updatePrescaler(1024); // Make sure the PWM prescaler is set correctly.
        }

        /// <summary>
        /// Update servo locations
        /// </summary>
        /// <param name="ch1">Location of servo connected to channel A ( in degrees ) - pin4</param>
        /// <param name="ch2">Location of servo connected to channel B ( in degrees ) - pin1</param>
        public void servo_updateLocation(byte ch1, byte ch2)
        {
            ch1 = (byte)((((ch1 / RANGE) * (MAX_LIMIT - MIN_LIMIT)) + MIN_LIMIT) / STEP_SIZE);
            ch2 = (byte)((((ch2 / RANGE) * (MAX_LIMIT - MIN_LIMIT)) + MIN_LIMIT) / STEP_SIZE);
            pwm_updateCompare(ch1, ch2);
        }

        #endregion

    }
}
