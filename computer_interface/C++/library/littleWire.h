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
/// General Purpose Pins
#define PIN0	0
#define PIN1	1
#define PIN2	2
#define PIN3	5

/// ADC Channels
#define ADC_RESET_PIN	0
#define ADC_SCK_PIN	1
#define ADC_TEMP_SENS	2

/// PWM Pins
#define PWM0	PIN0
#define PWM1	PIN1

/// Aliases
#define ADC0	ADC_RESET_PIN
#define ADC1	ADC_SCK_PIN
#define ADC2	ADC_TEMP_SENS
#define PWMA	PWM0
#define PWMB	PWM1

/// 'AVR ISP' Pins
#define SCK_PIN		PIN2
#define MISO_PIN	PIN1
#define MOSI_PIN	PIN0
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

#define HIGH	1
#define LOW	0


/**
 * littleWire Class.
 * @brief littleWire class
 */
class littleWire
{

private:
	char rxBuffer[RX_BUFFER_SIZE];
	usb_dev_handle* lwHandle;

public:
	/**
	 * Constructor
	 *
	 * @param (none)
	 * @return (none)
	 */
	littleWire();

	/**
	 * Destructor
	 *
	 * @param (none)
	 * @return (none)
	 */
	~littleWire();


	/*! \addtogroup Pin
	 *  Pin functions
	 *  @{
	 */

	/**
	 * Set pin as input/output
	 *
	 * @param pin Pin number
	 * @param mode Mode of pin (\b INPUT or \b OUTPUT)
	 * @return (none)
	 */
	void pinMode(unsigned char pin, unsigned char mode);

	/**
	 * Set pin value
	 *
	 * @param pin Pin name
	 * @param state Pin state (\b HIGH or \b LOW)
	 * @return (none)
	 */
	void digitalWrite(unsigned char pin, unsigned char state);

	/**
	 * 
	 *
	 * @param
	 * @param
	 * @return (none)
	 */
	unsigned char digitalRead(unsigned char pin);

	/**
	 * 
	 *
	 * @param
	 * @param
	 * @return (none)
	 */
	unsigned int analogRead(unsigned char channel);
	/*! @} */


	/*! \addtogroup PWM
	 *  PWM Functions
	 *  @{
	 */

	/**
	 * 
	 *
	 * @param
	 * @param
	 * @return (none)
	 */
	void pwm_init();

	/**
	 * 
	 *
	 * @param
	 * @param
	 * @return (none)
	 */
	void pwm_stop();

	/**
	 * 
	 *
	 * @param
	 * @param
	 * @return (none)
	 */
	void pwm_updateCompare(unsigned char channelA, unsigned char channelB);

	/**
	 * 
	 *
	 * @param
	 * @param
	 * @return (none)
	 */
	void pwm_updatePrescaler(unsigned int value);
	/*! @} */


	/*! \addtogroup SPI
	 *  SPI functions
	 *  @{
	 */
	void spi_init();

	/**
	 * 
	 *
	 * @param
	 * @param
	 * @return (none)
	 */
	unsigned char spi_sendMessage(unsigned char message);

	/**
	 * 
	 *
	 * @param
	 * @param
	 * @return (none)
	 */
	void spi_sendMessageMulti(unsigned char* sendBuffer, unsigned char* inputBuffer, unsigned char length, unsigned char mode);

	/**
	 * 
	 *
	 * @param
	 * @param
	 * @return (none)
	 */
	void spi_updateDelay(unsigned int duration);
	/*! @} */


	/*! \addtogroup I2C
	 *  I2C functions
	 *  @{
	 */
	void i2c_init();

	/**
	 * 
	 *
	 * @param
	 * @param
	 * @return (none)
	 */
	void i2c_beginTransmission(unsigned char address);

	/**
	 * 
	 *
	 * @param
	 * @param
	 * @return (none)
	 */
	void i2c_send(unsigned char message);

	/**
	 * 
	 *
	 * @param
	 * @param
	 * @return (none)
	 */
	void i2c_endTransmission();

	/**
	 * 
	 *
	 * @param
	 * @param
	 * @return (none)
	 */
	void i2c_requestFrom(unsigned char address,unsigned char numBytes,unsigned char* responseBuffer);
	/*! @} */


	/*! \addtogroup Servo
	 *  Servo functions
	 *  @{
	 */

	/**
	 * 
	 *
	 * @param
	 * @param
	 * @return (none)
	 */
	void servo_init();

	/**
	 * Update servo locations
	 *
	 * @param locationChannelA Location of servo connected to channel A
	 * @param locationChannelB Location of servo connected to channel B
	 * @return (none)
	 */
	void servo_updateLocation(unsigned char locationChannelA, unsigned char locationChannelB);
	/*! @} */

};

//} //namespace


/*! \mainpage Library Documentation
 *
 * \section intro_sec Introduction
 *
 * This is the introduction.
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Copy files
 *  
 * etc...
 */
#endif

