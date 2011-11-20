========================================
= Short story  
========================================
- When i saw simpleavr's implementation of usbtiny on attiny45 , i thought it would be cool if i make a kit version of this with a minimal form factor. Then i designed a PCB and sent for first prototype. Later on i thought, if i want to sell this, it would be much cooler ,and more suitable with "Open Source Hardware" concept, if i bring this project one step ahead. So i tried to fit anything extra to the device and this project came out.

========================================
= What is this?
========================================
- usbtiny compatable AVR programmer in minimal form factor with all through hole components
- Computer controllable (via USB) additional features: 
    - 4 channel digital input / output
    - ADC with 10 bit resolution  
    - 2 paralel hardware PWM outputs  
    - SPI interface

========================================
= What is pinout?
========================================
Name	port	pinNumber	PWM			GPIO	ADC
-----	-----	---------	----------	------	---------
RESET	PORTB	5			-			5		Channel_0*
MOSI	PORTB	0			Channel_A	0		-
MISO	PORTB	1			Channel_B	1		-
SCK		PORTB	2			-			2		Channel_1

* Analog reading from RESET pin isn't stable but supported. Use it at your risk.  

========================================
= Possible usages
========================================
- AVR programming /summon captainObvious 
- LED dimming, color mixing  
- General servo driving   
- Pan and tilt servo control of a webcam for face tracking  
- With additional motor driver IC, 2 wheel robot driving   
- Analog voltage recording and plotting  

========================================
= What is included?
========================================
- Eagle schematic / layout files. Also in PDF format.
- Microcontroller firmware files and makefile  
- C# class for Windows USB interface  
- C# example project for analog voltage plotting based on zedGraph  
- C function/header files for command line USB interface for Windows and Linux  

========================================
= Reference projects
========================================
- This project is proudly and heavily based on 
  - Original usbtiny project
     http://www.xs4all.nl/~dicks/avr/usbtiny/ 
  - Ladyada version of usbtiny
     http://www.ladyada.net/make/usbtinyisp/
  - simpleavr's implementation of usbTiny on attiny45 microcontroller
     http://www.simpleavr.com/avr/vusbtiny