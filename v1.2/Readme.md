
![Little wire beta in action](https://github.com/kehribar/Little-Wire/raw/master/v1.1/Little-Wire_beta.jpg)

# Little Wire - v1.2 - ( in progress … )

Little Wire is multi-featured USB controlled open source hardware tool packed in a minimal form factor designed by ihsan Kehribar. This documentation explains the features of the Little Wire via computer inteface library written in C language. Details of the library can be found in the `littleWire.h` header file.

There are also official C++, C# and Processing libraries for interfacing with the current or previous firmware versions of the Little Wire. At some level, maintaining more than one language become a problem; therefore currently the C language library will be the only official and supported interface library. On the other hand there is a good supported Ruby library exists for some features of the Little Wire. Users can take a look at it.

## Basic features

### AVR Programming

Little Wire is an **usbtinyisp** compatible AVR programmer, right out of the box. 2x3 pinheader at the Little Wire is standard AVR-ISP header compatible. Individual pin mapping for this mode is:

	PIN1 -> MISO
	PIN2 -> SCK
	PIN3 -> RESET
	PIN4 -> MOSI

### Digital I/O
Little Wire has 4 available GPIO pins. Any of the pins can be used for writing/reading. Logic level is +5 volts. Don’t apply more than +5 volts to any GPIO pin.
 
* Logic output
	
		/* First, configure pin as output */
		pinMode(lw,PIN1,OUTPUT);
		
		/* Set the logic level of the pin to a desired level */
		digitalWrite(lw,PIN1,HIGH);
		
	For a complete example, refer to the `blink.c` example file.
  
* Logic input

		/* First configure pin as input */
		pinMode(lw,PIN2,INPUT);
		
		/* Optionally, enable or disable the internal pullup resistor */
		internalPullup(lw,PIN2,ENABLE);
		
		/* Read the level of the digital pin */
		if(digitalRead(lw,PIN2) == LOW)
		{
			/* do something ... */
		}		
	
	For a complete example, refer to the `button.c` example file.

### ADC

Little Wire has 10 bit ADC. Reference voltage is selectable. PIN2 and PIN3 are available for ADC feature. Reading analog voltage from PIN3 is noisy; use it at your own risk. PIN2 is suggested for generic ADC usage.

Moreover, the Little Wire supports reading the internal temperature sensor of the Attiny85. You can refer to Attiny85 datasheet for the details of this internal sensor.

* Analog read
	
		/* Select ADC reference */
		/* Those definetly needs calibration for precision measurements. */
		analog_init(lw,VREF_VCC);
		// analog_init(lw,VREF_1100mV);
		// analog_init(lw,VREF_2560mV;	
		
		/* Read 10 bit unsigned ADC value */
		adcValue = analogRead(lw, ADC_PIN2);
		
	For a complete example, refer to the `adc.c` example file.

### PWM
	
#### Hardware PWM

Little Wire can generate hardware PWM signals from PIN1 and PIN4. PWM frequency is adjustabla via prescaler setting. Valid prescaler values are *1, 8, 64, 256* and *1024*. Higher the prescaler value, slower the PWM freuqency. 

* Hardware PWM

		/* Init the PWM module */
		pwm_init(lw);
		
		/* Set the maximum PWM frequency */
		pwm_updatePrescaler(lw,1);	
		
		/* Update the PWM values when needed */
		pwm_updateCompare(lw,val1,val2);

	For a complete example, refer to the `hardwarePWM.c` example file.

##### Servo driving
Servo driving is a subset library for the hardware PWM module. It adjusts the PWM output signals for servo diriving.

* Servo driving

		/* Init the servo specialized hardware PWM module */
		servo_init(lw);

		/* Set two servo channels to the opposite positions */
		/* Location value is in degrees */
		servo_updateLocation(lw,currentLocation,(180 - currentLocation));
		
	For a complete example, refer to the `servo.c` example file.

#### Software PWM

Little Wire can generate software PWM signals from PIN1, PIN2 and PIN4. Usefull for driving an RGB LED.

* RGB LED driving
		 
		 /* First, enable the software PWM module */
		 softPWM_state(lw,ENABLE);
		 
		 /* Update the PWM values when needed ... */
		 /* r,g and b are the three PWM values for an RGB LED */
		 softPWM_write(lw,r,g,b);

	For a complete example, refer to the `softPWM.c` example file.

### SPI 

Little Wire can act like an SPI master device. Pin mapping for SPI mode is as follows: 
	
	PIN1 -> Data out
	PIN2 -> Clock
	PIN3 -> Chip select
	PIN4 -> Data in
	
* Communication with manual chip select 

		/* Set the chip select pin as output. */
		pinMode(lw,PIN3,OUTPUT);
		
		/* Set the value of the chip select pin high initially */
		digitalWrite(lw,PIN3,HIGH);
		
		/* Change this according to your device. If your device doesn't respond, try to increase the delay */
		/* This is the maximum speed. */
		spi_updateDelay(lw,0); 

		/* Set the chip select pin low before sending the data */
		digitalWrite(lw,PIN3,LOW);

		/* Fill the SPI send buffer */
		sendBuffer[0] = 0;
		sendBuffer[1] = 1;
		sendBuffer[2] = 2;
		sendBuffer[3] = 3;						
		
		/* Send the initial bytes ... Maximum length for SPI message buffer is 4 */
		spi_sendMessage(lw,sendBuffer,receiveBuffer,4,MANUAL_CS);
		
		/* Fill the SPI send buffer */
		sendBuffer[0] = 4;
		sendBuffer[1] = 5;
		sendBuffer[2] = 6;						
		
		/* Send the rest of the bytes ... This time we set the length as 3 */
		spi_sendMessage(lw,sendBuffer,receiveBuffer,3,MANUAL_CS);
	
		/* Set the chip select pin high again */
		digitalWrite(lw,PIN3,HIGH);

* Communication with automatic chip select 

		/* Set the chip select pin as output. */
		pinMode(lw,PIN3,OUTPUT);
		
		/* Set the value of the chip select pin high initially */
		digitalWrite(lw,PIN3,HIGH);
		
		/* Change this according to your device. If your device doesn't respond, try to increase the delay */
		/* This is the maximum speed. */
		spi_updateDelay(lw,0); 
		
		/* Fill the SPI send buffer */
		sendBuffer[0] = 1;
		sendBuffer[1] = 2;
		sendBuffer[2] = 3;						
		
		/* Send the bytes. Chip select pin levels are automatically handled by the hardware */
		spi_sendMessage(lw,sendBuffer,receiveBuffer,3,AUTO_CS);
		
	For a complete example, refer to the `spi_LTC1448.c` example file.

### I2C

Little Wire can act like an I2C master device. Pin mapping for I2C mode is as follows: 

	PIN2 -> SCL
	PIN4 -> SDA
	
* I2C BlinkM driving

		/* Init i2c module */
		i2c_init(lw);
		
		/* Set the i2c signal delay according to your device. */
		i2c_updateDelay(lw,10);
		
		/* Initiate the communication in WRITE mode */
		i2c_start(lw,7bit_address,WRITE);
		
		/* Fill the i2c send buffer */	
		/* BlinkM 'Go to custom HSV color' command */
		myBuffer[0] = 'h';
		myBuffer[1] = 123;
		myBuffer[2] = 215;
		myBuffer[3] = 155;		

		/* Write 4 bytes to the slave device. Maximum length is 4 */
		i2c_write(lw,myBuffer,4,END_WITH_STOP);
		
		/* Wait a little ... */
		delay(10);

		/* Initiate the communication in WRITE mode */
		i2c_start(lw,7bit_address,WRITE);	
		
		/* Fill the i2c buffer */
		/* BlinkM 'Get the current RGB color' command */
		myBuffer[0] = 'g'; 
		
		/* Write 1 byte to the slave device */
		i2c_write(lw,myBuffer,1,END_WITH_STOP);
		
		/* Wait a little ... */
		delay(10);
		
		/* Initiate the communication in READ mode */
		i2c_start(lw,7bit_address,WRITE);
		
		/* Read 3 bytes from slave ... */ 
		i2c_read(lw,myBuffer,3,END_WITH_STOP);

	For a complete example, refer to the `i2c_blinkM.c` example file.

* Address search

		/* Init i2c module */
		i2c_init(lw);
		
		/* Set the i2c signal delay according to your device. */
		i2c_updateDelay(lw,10);
	
		printf("> Address search ... \n");
	
		/* Search for all possible address values */
		for(i=0;i<128;i++)
		{
			/* Try to initiate communication */
			rc = i2c_start(lw,i,WRITE);	
			
			/* Return value is 1 if a valid ACK is received */
			if(rc == 1)
			{
				printf("> Found device at %3d\n",i);
			}
		}
	
	For a complete example, refer to the `i2c_blinkM.c` example file.


### Onewire

Little Wire can act like an 1-wire master device. Pin mapping for 1-wire mode is as follows:

	PIN2 -> Data

* DS1820 type digital sensor

		// send reset signal and read the presence value 
		// if this returns zero, sensor is unplugged 
		if(!onewire_resetPulse(lw)) 		
		{
			printf("> Sensor is unplugged!\n");
			return 0;
		}

		// onewire_writeByte(0xCC); /* SKIP ROM command */
		onewire_writeByte(lw,0x55); /* MATCH ROM command *		
		onewire_writeByte(lw,ROM_NO[0]); /* Address begins */
		onewire_writeByte(lw,ROM_NO[1]);
		onewire_writeByte(lw,ROM_NO[2]);
		onewire_writeByte(lw,ROM_NO[3]);
		onewire_writeByte(lw,ROM_NO[4]);
		onewire_writeByte(lw,ROM_NO[5]);
		onewire_writeByte(lw,ROM_NO[6]);
		onewire_writeByte(lw,ROM_NO[7]); /* Address ends */	
		onewire_writeByte(lw,0x44); /* CONVERT T command */

		// wait for conversion - this is important, if you allways get 85 deg. C, increase tis value 
		delay(650);

		// send reset signal and read the presence value
		if(!onewire_resetPulse(lw)) // if this returns zero, sensor is unplugged		
		{
			printf("> Sensor is unplugged!\n");
			return 0;
		}

		// onewire_writeByte(0xCC); /* SKIP ROM command */
		onewire_writeByte(lw,0x55); /* MATCH ROM command */		
		onewire_writeByte(lw,ROM_NO[0]); /* Address begins */
		onewire_writeByte(lw,ROM_NO[1]);
		onewire_writeByte(lw,ROM_NO[2]);
		onewire_writeByte(lw,ROM_NO[3]);
		onewire_writeByte(lw,ROM_NO[4]);
		onewire_writeByte(lw,ROM_NO[5]);
		onewire_writeByte(lw,ROM_NO[6]);
		onewire_writeByte(lw,ROM_NO[7]); /* Address ends */	
		onewire_writeByte(lw,0xBE); /* Read register (scratchpad) command */
		
		for(i=0;i<9;i++) //read 9 bytes from SCRATCHPAD
		{	
	        scratch = onewire_readByte(lw);
	        
	        if (i == 0) // LSB temperature register byte (0)
	                templow = scratch;
	        if (i == 1) // MSB temperature register byte (1)
	                temphigh = scratch;
		}
		
		tempdecimal = 0;

		if ((templow & 0x01) == 0x01)
		{
			tempdecimal += 625;	
		}
		
		if ((templow & 0x02) == 0x02)
		{
			tempdecimal += 1250;	
		}
		
		if ((templow & 0x04) == 0x04)
		{
			tempdecimal += 2500;	
		}
		
		if ((templow & 0x08) == 0x08)
		{
			tempdecimal += 5000;	
		}						

		tempdecimal /= 10;
        
        printf("> TEMP %c%d.%d deg. C\n", ((temphigh & 0x80) == 0x80) ? '-' : ' ', ((templow & 0xf0) >> 4) | ((temphigh & 0x07) << 4), tempdecimal);


* Address search

		printf("> Address search ... \n");
		
		if(onewire_firstAddress(lw))
		{
			printf("> Addr#%d:  %X\t",i,ROM_NO[0]);
			for(t=0;t<7;t++)
				printf("%X\t",ROM_NO[t+1]);			
			printf("\n");
			i = 1;
		}
		
		while(onewire_nextAddress(lw))
		{
			printf("> Addr#%d:  %X\t",i,ROM_NO[0]);
			for(t=0;t<7;t++)
				printf("%X\t",ROM_NO[t+1]);			
			printf("\n");
			i++;
		}
		
		if(i > 0)
		{
			printf("> End of search with %d onewire device(s) found\r\n",i);
		}	
		else
		{
			printf("> No onewire device has been found!\n");
			return 0;
		}

	For a complete example, refer to the `onewire.c` example file.

## Advanced features

	To be continued …
	
## Reference projects

- This project is proudly and heavily based on: 
    - Original usbtiny project
        http://www.xs4all.nl/~dicks/avr/usbtiny/ 
    - Ladyada version of usbtiny
        http://www.ladyada.net/make/usbtinyisp/
    - simpleavr's implementation of usbTiny on attiny45 microcontroller
        http://www.simpleavr.com/avr/vusbtiny
    - Libusb library for C , C++ interface
        http://www.libusb.org/
    - Libusbdotnet library for C# inteface
        http://libusbdotnet.sourceforge.net/V2/Index.html
    - Libusbjava library for Processing interface
        http://libusbjava.sourceforge.net
    - "fast tiny & mega UART bootloader" project from AVR freaks
        http://tinyurl.com/fastTinyAndMega-UARTbootloader
    - Micronucleus bootloader: https://github.com/Bluebie/micronucleus-t85
        
## Please note: 
	Do not use the Adafruit USB VID/PID without written permission from Adafruit Industries, LLC and 
	Limor "Ladyada" Fried (support@adafruit.com). Permission is granted for littlewire, Ihsan Kehribar
	and Seeed Studio by Adafruit Industries, LLC to use the Adafruit USB VID/PID for littlewire 
	(SKU:AVR06071P)
