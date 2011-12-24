# Makefile for Little-Wire

CC=gcc

ifeq ($(shell uname), Linux)
  USBFLAGS = `libusb-config --cflags`
  USBLIBS = `libusb-config --libs`
  EXE_SUFFIX =
  OSFLAG = -D LINUX
else
  USBFLAGS = -I C:\MinGW\include
  USBLIBS = -L C:\MinGW\lib -lusb
  EXE_SUFFIX = .exe
  OSFLAG = -D WIN
endif

CFLAGS  = $(USBFLAGS) $(USBLIBS) -Ilibrary -O -g $(OSFLAG)

EXAMPLES = adc

.PHONY:	clean library

all: library $(EXAMPLES)

library:
	$(CC) $(CFLAGS) -c library/littleWire.c
	$(CC) $(CFLAGS) -c library/opendevice.c

$(EXAMPLES): littleWire.o opendevice.o
	$(CC) $(CFLAGS) -o $(EXAMPLES)$(EXE_SUFFIX) examples/$(EXAMPLES).c littleWire.o opendevice.o

clean:
	rm -f $(EXAMPLES) *.o *.exe
