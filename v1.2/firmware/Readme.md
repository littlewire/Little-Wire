To burn the bootloader firmware: 

	avrdude -c usbtiny -p t85 -U flash:w:micronucleus-1.02.hex -U lfuse:w:0xe1:m -U hfuse:w:0x5d:m -U efuse:w:0xfe:m
 	
To upload littlewire firmware over the bootloader:
	
	micronucleus main.hex

To upload cdc232.hex (USB to serial firmware) over the bootloader:

	micronucleus cdc232.hex
	
Bootloader usage:
	
	1- Disconnect the Little Wire from the USB port
	2- Launch the bootloader program with neccessary parameters
	3- Insert the device into the USB port