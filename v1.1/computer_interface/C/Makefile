
# Makefile for Little-Wire
# Created by Omer Kilic <omerkilic@gmail.com>

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

LWLIBS = opendevice littleWire littleWire_util littleWire_servo
EXAMPLES = pic24f adc blink button servo i2c_blinkM spi_LTC1448 onewire softPWM hardwarePWM debugConsole lwbuttond

.PHONY:	clean library

all: library $(EXAMPLES)

library: $(LWLIBS)

$(LWLIBS):
	@echo Building library: $@...
	$(CC) $(CFLAGS) -c library/$@.c 

$(EXAMPLES): $(addsuffix .o, $(LWLIBS))
	@echo Building example: $@...
	$(CC) $(CFLAGS) -o $@$(EXE_SUFFIX) examples/$@.c $^ $(LIBS)

clean:
	rm -f $(EXAMPLES)$(EXE_SUFFIX) *.o *.exe
