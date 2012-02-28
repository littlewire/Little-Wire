To burn the bootloader firmware: 

	avrdude -c usbtiny -p t85 -U flash:w:myBootLoader.hex -U lfuse:w:0xe1:m -U hfuse:w:0x5d:m -U efuse:w:0xfe:m

To burn the (bootloader+main) combined firmware: 

	avrdude -c usbtiny -p t85 -U flash:w:combined.hex -U lfuse:w:0xe1:m -U hfuse:w:0x5d:m -U efuse:w:0xfe:m
 
To read the firmware from the device:

	avrdude -c usbtiny -p t85 -U flash:r:myFirmware.hex:r
	
To upload main.hex over the bootloader: (for com3)
	
	fboot.exe -c3 -pmain.hex

To upload cdc232.hex (USB to serial firmware) over the bootloader: (for com3)

	fboot.exe -c3 -pcdc232.hex