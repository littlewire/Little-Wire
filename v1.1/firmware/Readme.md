To burn the bootloader firmware: 

	avrdude -c usbtiny -p t85 -U flash:w:myBootLoader.hex -U lfuse:w:0xe1:m -U hfuse:w:0x5d:m -U efuse:w:0xfe:m

To burn the (bootloader+main) combined firmware: 

	avrdude -c usbtiny -p t85 -U flash:w:combined_v1-1.hex -U lfuse:w:0xe1:m -U hfuse:w:0x5d:m -U efuse:w:0xfe:m
 	
To upload main.hex over the bootloader: (for Com3 / ttyUSB0)
	
	fboot.exe -c3 -pmain.hex	(for windows)
	./bootloader -d /dev/ttyUSB0 -b 9600 -p main.hex	(for linux)

To upload cdc232.hex (USB to serial firmware) over the bootloader: (for Com3 / ttyUSB0)

	fboot.exe -c3 -pcdc232.hex	(for windows)
	./bootloader -d /dev/ttyUSB0 -b 9600 -p cdc232.hex	(for linux)
	
Bootloader usage:
	
	1- Disconnect the Little Wire from the USB port
	2- Connect the TX/RX pins between the Little Wire and your serial port adapter
	3- Launch the fboot/bootloader program with neccessary parameters
	4- After the program connected to the com port, plug the Little Wire into the USB port
	5- The new firmware should have been uploaded at this step. If anything goes wrong; 
		* First, check that you connected the cables properly
		* Second, unplug the Little Wire and repeat the uploading process. Even the wiring is true, sometimes the bootloader works at the second/third trial.
