# Makefile for Little-Wire

CC=gcc

# FIXME: Need to add OSX stuff
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

LIBS    = $(USBLIBS)
INCLUDE = library
CFLAGS  = $(USBFLAGS) $(LIBS) -I$(INCLUDE) -O -g $(OSFLAG)

EXAMPLES = adc blink button servo

.PHONY:	clean library

all: library $(EXAMPLES)

library:
	@echo Building libraries...
	$(CC) $(CFLAGS) -c library/littleWire.c 
	$(CC) $(CFLAGS) -c library/littleWire_servo.c 
	$(CC) $(CFLAGS) -c library/opendevice.c 

$(EXAMPLES): littleWire.o opendevice.o littleWire_servo.o
	@echo Building example:$(@)...
	$(CC) $(CFLAGS) -o $@$(EXE_SUFFIX) examples/$@.c $^ $(LIBS)

clean:
	rm -f $(EXAMPLES)$(EXE_SUFFIX) *.o *.exe
