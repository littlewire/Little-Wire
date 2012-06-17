using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LibUsbDotNet;
using LibUsbDotNet.Main;

namespace  littleWireLib
{
    public class littleWire
    {
        // Pin mappings
        public static byte PIN1 = 1;
        public static byte PIN2 = 2;
        public static byte PIN3 = 5;
        public static byte PIN4 = 0;

        // Pin directions
        public static byte IN = 1;
        public static byte OUT = 0;

        // Pin directions (aliases)
        public static byte INPUT = IN;
        public static byte OUTPUT = OUT;

        // Pin states
        public static byte HIGH = 1;
        public static byte LOW = 0;

        private static UsbDeviceFinder MyUsbFinder;
        private static UsbDevice MyUsbDevice;
        private UsbSetupPacket MySetupPacket;

        /// <summary>
        /// Simple constructor
        /// </summary>
        public littleWire()
        {
            MyUsbFinder = new UsbDeviceFinder(0x1781, 0x0C9F);
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
        /// Set a GPIO pin High/Low
        /// </summary>
        /// <param name="pin">Pin number</param>
        /// <param name="state">1 for High, 0 for Low</param>
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
                MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
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
                MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
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
            public void setPinInput(byte pin)
            {
                byte[] buffer_ = new byte[8];
                int whatIsThis = 8;
                MySetupPacket = new UsbSetupPacket(0xC0, 13, pin, 0, 8);
                MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
            }
            /// <summary>
            /// Set a GPIO pin output
            /// </summary>
            /// <param name="pin">Pin number</param>
            public void setPinOutput(byte pin)
            {
                byte[] buffer_ = new byte[8];
                int whatIsThis = 8;
                MySetupPacket = new UsbSetupPacket(0xC0, 14, pin, 0, 8);
                MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
            }
        
        /// <summary>
        /// Read a state of a GPIO pin
        /// </summary>
        /// <param name="pin">Pin number</param>
        /// <returns>1 for HIGH, 0 for LOW</returns>
        public byte digitalRead(byte pin)
        {
            byte[] buffer_ = new byte[8];
            int whatIsThis = 8;
            MySetupPacket = new UsbSetupPacket(0xC0, 20, pin, 0, 8);
            MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
            return buffer_[0];
        }

        /// <summary>
        /// Read analog voltage from a spesific channel
        /// </summary>
        /// <param name="channel">0 for RESET pin, 1 for SCK pin, 2 for internal Temperature sensor</param>
        /// <returns>Analog voltage in 10bit resoultion</returns>
        public ushort analogRead(byte channel)
        {
            byte[] buffer_ = new byte[8];
            int whatIsThis = 8;
            MySetupPacket = new UsbSetupPacket(0xC0, 15, channel, 0, 8);
            MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
            return (ushort)((buffer_[1] <<8) + (buffer_[0]));
        }
        
        /// <summary>
        /// Initialize the Pwm module on the device
        /// </summary>
        public void initPwm()
        {
            byte[] buffer_ = new byte[8];
            int whatIsThis = 8;
            MySetupPacket = new UsbSetupPacket(0xC0, 16, 0, 0, 8);
            MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
        }
            /// <summary>
            /// Update the compare values of Pwm outputs
            /// </summary>
            /// <param name="channelA">Channel A</param>
            /// <param name="channelB">Channel B</param>
            public void updatePwmCompare(byte channelA, byte channelB)
            {
                byte[] buffer_ = new byte[8];
                int whatIsThis = 8;
                MySetupPacket = new UsbSetupPacket(0xC0, 17, channelA, channelB, 8);
                MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
            }
            /// <summary>
            /// Change the Pwm prescaler. Default: 1024
            /// </summary>
            /// <param name="value">1024/256/64/8/1</param>
            public void updatePwmPrescale(UInt16 value)
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
                MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
            }

        /// <summary>
        /// Send a one byte SPI message.
        /// </summary>
        /// <param name="message">Message to send</param>
        public byte sendSpiMessage(byte message)
        {
            byte[] buffer_ = new byte[8];
            int whatIsThis = 8;
            MySetupPacket = new UsbSetupPacket(0xC0, 21, message, 0, 8);
            MyUsbDevice.ControlTransfer(ref MySetupPacket, buffer_, 8, out whatIsThis);
            return buffer_[0];
        }

    }
}
