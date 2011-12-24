# Makefile for Little-Wire

# Linux
CC=gcc
USBFLAGS = `libusb-config --cflags`
USBLIBS = `libusb-config --libs`
OSFLAG = -D LINUX
CFLAGS  = $(USBFLAGS) $(USBLIBS) -Ilibrary -O -g $(OSFLAG)

EXAMPLES = adc

.PHONY:	clean library

all: library $(EXAMPLES)

library:
	$(CC) $(CFLAGS) -c library/littleWire.c
	$(CC) $(CFLAGS) -c library/opendevice.c

$(EXAMPLES): littleWire.o opendevice.o
	$(CC) $(CFLAGS) -o $(EXAMPLES) examples/$(EXAMPLES).c littleWire.o opendevice.o

clean:
	rm -f $(EXAMPLES) *.o *.exe
