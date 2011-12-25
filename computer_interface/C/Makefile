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
LIBS	= $(USBLIBS)

EXAMPLES = adc blink

.PHONY:	clean library

all: library $(EXAMPLES)

library:
	@echo Building libraries...
	$(CC) $(CFLAGS) -c library/littleWire.c 
	$(CC) $(CFLAGS) -c library/opendevice.c 
	@echo Building examples...
$(EXAMPLES): littleWire.o opendevice.o
	@echo Building $(@)...
	$(CC) $(CFLAGS) -o $@$(EXE_SUFFIX) examples/$@.c $^ $(LIBS)

clean:
	rm -f $(EXAMPLES) *.o *.exe
