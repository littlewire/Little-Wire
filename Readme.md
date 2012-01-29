![Little wire beta in action](./Little-Wire/raw/master/Little-Wire_beta.jpg)

<center>http://kehribar.me/projects/Little-Wire/<center>

### Short story  

- When i saw simpleavr's implementation of usbtiny on attiny45 , i thought it would be cool if i make a kit version of this with a minimal form factor. Then i designed a PCB and sent for first prototype. Later on i thought, if i want to sell this, it would be much cooler ,and more suitable with "Open Source Hardware" concept, if i bring this project one step ahead. So i tried to fit anything extra to the device and this project came out.

### What is this?

- usbtiny compatable AVR programmer in minimal form factor with all through hole components
- Computer controllable (via USB) additional features: 
    - 4 channel digital input / output
    - ADC with 10 bit resolution
    - 2 paralel hardware PWM outputs  
    - SPI interface
    - I2C interface
- Includes onboard serial bootloader for firmware upgrades! 
- USB to UART converter , by loading an other firmware (CDC-232 port)

### Possible usages

- AVR programming <i>/summon captainObvious</i>
- LED dimming, color mixing  
- General servo driving   
- Pan and tilt servo control of a webcam for face tracking  
- With additional motor driver IC, 2 wheel robot driving   
- Analog voltage recording and plotting
- Controlling LED matrix displays by using SPI module to drive shift registers
- USB to SPI bridge
- USB to I2C bridge
- USB to UART bridge
- General purpose V-USB developement board

### What is included?

- Eagle schematic / layout files. Also in PDF format.
- Microcontroller firmware files and makefile  
- Bootloader source files
- C function/header files for command line USB interface for Windows and Linux
- C++ class for Windows and Linux 
- C# class for Windows USB interface  
- Example C projects for almost every feature of the board   
- Example C++ projects for almost every feature of the board  
- Example C# project for analog voltage plotting based on zedGraph
- Example C# projects for almost every feature of the board
- Processing library for Windows ( in developement ... ) 


### Reference projects

- This project is proudly and heavily based on 
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
    - USIi2c library for I2C implementation
        http://arduino.cc/playground/Code/USIi2c