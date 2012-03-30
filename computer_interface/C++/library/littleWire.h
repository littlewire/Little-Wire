#ifndef LITTLEWIRE_H
#define LITTLEWIRE_H

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

extern "C" {
  #if defined(LINUX)
    #include <usb.h>
  #else
    #include <lusb0_usb.h>
  #endif

  #include "opendevice.h"
}

//namespace LittleWire
//{

/**
 * USB constants
 */
#define	VENDOR_ID	0x1781
#define	PRODUCT_ID	0x0c9f
#define USB_TIMEOUT	5000
#define	RX_BUFFER_SIZE	64

/**
 * Pin definitions
 */
// General Purpose Pins
#define PIN1	1
#define PIN2	2
#define PIN3	5
#define PIN4	0

// ADC Channels
#define ADC_PIN3		0
#define ADC_PIN2		1
#define ADC_TEMP_SENS	2

// PWM Pins
#define PWM1	PIN4
#define PWM2	PIN1

// Aliases
#define ADC0	ADC_PIN3
#define ADC1	ADC_PIN2
#define ADC2	ADC_TEMP_SENS
#define PWMA	PWM1
#define PWMB	PWM2

// 'AVR ISP' Pins
#define SCK_PIN		PIN2
#define MISO_PIN	PIN1
#define MOSI_PIN	PIN4
#define RESET_PIN	PIN3

/**
 * Servo constants
 */
#define STEP_SIZE	0.062	// in miliseconds
#define MIN_LIMIT	0.8	// in miliseconds
#define MAX_LIMIT	2.2	// in miliseconds
#define RANGE		180	// in degrees

/**
 * Useful constants
 */
#define INPUT	1
#define OUTPUT	0

#define AUTO_CS	1
#define MANUAL_CS	0

#define HIGH	1
#define LOW	0


/**
 * littleWire Class.
 * @brief littleWire class
 */
class littleWire
{

private:
	unsigned char rxBuffer[RX_BUFFER_SIZE];
	usb_dev_handle* lwHandle;

public:
	/**
	 * Simple Constructor
	 *
	 * @param (none)
	 * @return (none)
	 */
	littleWire();

	/**
	 * Simple Destructor
	 *
	 * @param (none)
	 * @return (none)
	 */
	~littleWire();


	/*! \addtogroup GPIO
	 *  GPIO functions
	 *  @{
	 */

	/**
	 * Set pin as input/output
	 *
	 * @param pin Pin name (\b PIN1 , \b PIN2 , \b PIN3 or \b PIN4 )
	 * @param mode Mode of pin (\b INPUT or \b OUTPUT)
	 * @return (none)
	 */
	void pinMode(unsigned char pin, unsigned char mode);

	/**
	 * Set pin value
	 *
	 * @param pin Pin name (\b PIN1 , \b PIN2 , \b PIN3 or \b PIN4 )
	 * @param state Pin state (\b HIGH or \b LOW)
	 * @return (none)
	 */
	void digitalWrite(unsigned char pin, unsigned char state);

	/**
	 * Read pin value
	 *
	 * @param pin Pin name (\b PIN1 , \b PIN2 , \b PIN3 or \b PIN4 )
	 * @return Pin state (\b HIGH or \b LOW)
	 */
	unsigned char digitalRead(unsigned char pin);
	/*! @} */

	/*! \addtogroup ADC
	 *  Analog to Digital Converter Function
	 *  @{
	 */
	
	/**
	 * Read analog voltage. Analog voltage reading from ADC_PIN3 isn't advised (it is a bit noisy) but supported. Use it at your own risk.
	 * \n For more about internal temperature sensor, look at the Attiny85 datasheet.
	 *
	 * @param channel Source of ADC reading (\b ADC_PIN2 , \b ADC_PIN3 or \b ADC_TEMP_SENS)
	 * @return 10 bit ADC result with reference to 5V
	 */
	unsigned int analogRead(unsigned char channel);
	
	/*! @} */

	/*! \addtogroup PWM
	 *  PWM Functions
	 *  @{
	 */

	/**
	 * Initialize PWM module on the Little-Wire 
	 *
	 * @return (none)
	 */
	void pwm_init();

	/**
	 * De-Initialize PWM module on the Little-Wire 
	 *
	 * @return (none)
	 */
	void pwm_stop();

	/**
	 * Update the compare values of the PWM output pins. Resolution is 8 bit.
	 *
	 * @param channelA Compare value of \b PWMA pin
	 * @param channelB Compare value of \b PWMB pin
	 * @return (none)
	 */
	void pwm_updateCompare(unsigned char channelA, unsigned char channelB);

	/**
	 * Update the prescaler of the PWM module. Adjust this value according to your need for speed in PWM output. Default is 1024. Lower prescale means higher frequency PWM output.
	 *
	 * @param value Presecaler value (\b 1024, \b 256, \b 64, \b 8 or \b 1) 
	 * @return (none)
	 */
	void pwm_updatePrescaler(unsigned int value);
	/*! @} */


	/*! \addtogroup SPI
	 *  SPI functions
	 *  @{
	 */
	 
	 /**
	 * Initialize the SPI module on the Little-Wire 
	 *
	 * @return (none)
	 */
	void spi_init();

	/**
	 * Send and receive a single byte SPI message. Chip select is always manual
	 *
	 * @param message Message to send
	 * @return Response byte
	 */
	unsigned char spi_sendMessage(unsigned char message);

	/**
	 * Send and receive multiple (up to 4 bytes) SPI messages. Chip select is automatic or manual.
	 * \n If automatic chip select is selected, \b RESET_PIN stays HIGH in idle time , and goes to LOW while sending the messages over SPI
	 *
	 * @param sendBuffer Pointer of the array which has your messages to send.
	 * @param inputBuffer Pointer of the array which you want to store SPI response messages.
	 * @param length Length of the messages. ( Maximum is \b 4)
	 * @param mode Chip select mode. (\b AUTO_CS or \b MANUAL_CS )
	 * @return (none)
	 */
	void spi_sendMessageMulti(unsigned char* sendBuffer, unsigned char* inputBuffer, unsigned char length, unsigned char mode);

	/**
	 * Change the SPI message frequency by adjusting delay duration. By default, Little-Wire sends the SPI messages with maximum speed. 
	 * \n If your hardware can't catch up with the speed, increase the duration value to lower the SPI speed.
	 *
	 * @param duration Amount of delay. 
	 * @return (none)
	 */
	void spi_updateDelay(unsigned int duration);
	/*! @} */


	/*! \addtogroup I2C
	 *  I2C functions.
	 *  @{
	 */
	 
	 /**
	 * Initialize the I2C module on the Little-Wire 
	 *
	 * @return (none)
	 */
	void i2c_init();

	/**
	 * Begin a read/write operation on I2C bus.
	 *
	 * @param address 7 bit slave address.
	 * @return (none)
	 */
	void i2c_beginTransmission(unsigned char address);

	/**
	 * Add a message to output buffer.
	 *
	 * @param message Message to send.
	 * @return (none)
	 */
	void i2c_send(unsigned char message);

	/**
	 * End the I2C transmission. 
	 * \n Actually, all the messages will be sent at once when you call this function.
	 *
	 * @return (none)
	 */
	void i2c_endTransmission();

	/**
	 * Request a response from a slave device.
	 *
	 * @param address 7 bit slave address.
	 * @param numBytes Length of the response you expect from the slave device.
	 * @param responseBuffer Pointer of the array which you want to store I2C response messages.
	 * @return (none)
	 */
	void i2c_requestFrom(unsigned char address,unsigned char numBytes,unsigned char* responseBuffer);
	/*! @} */


	/*! \addtogroup Servo
	 *  Servo functions. Higher level access to PWM module.
	 *  @{
	 */

	/**
	 * Initialize the PWM module on the Little-Wire with the Servo special settings.
	 *
	 * @return (none)
	 */
	void servo_init();

	/**
	 * Update servo locations 
	 *
	 * @param locationChannelA Location of servo connected to channel A ( in degrees )
	 * @param locationChannelB Location of servo connected to channel B ( in degrees )
	 * @return (none)
	 */
	void servo_updateLocation(unsigned char locationChannelA, unsigned char locationChannelB);
	/*! @} */

};

//} //namespace


/*! \mainpage Library Documentation
 * <img src="http://kehribar.me/projects/Little-Wire/little-wire.jpg" alt="Little-Wire beta in action" width="320px" height="240px">
 * <center> <i> http://kehribar.me/projects/Little-Wire/ </i> </center>
 * \section intro_sec Introduction
 *
 * This is the doxygen documentation of the C++ library of <a href="http://kehribar.me/projects/Little-Wire/">Little-Wire</a> project. 
 * Other language interfaces (C,C#,Processing) of the <a href="http://kehribar.me/projects/Little-Wire/">Little-Wire</a> uses mostly the same names and usages.
 * Most function names and usage are chosen as similar as possible to the <a href="http://arduino.cc">Arduino</a> library functions for to lower the learning curve.
 
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1 
 * Download Libusb
 * \subsection step2 Step 2
 * Copy the files under <i>library</i> to your working folder and include littleWire.h to your program.
 * \subsection step3 Step 3 
 * Look to the provided examples for further help.
 *  
 */
#endif

