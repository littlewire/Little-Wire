
# Makefile for Little-Wire
# Created by Omer Kilic <omerkilic@gmail.com>

CC=gcc

ifeq ($(shell uname), Linux)
	USBFLAGS = `libusb-config --cflags`
	USBLIBS = `libusb-config --libs`
	EXE_SUFFIX =
	OSFLAG = -D LINUX
else ifeq ($(shell uname), Darwin)
	USBFLAGS = `libusb-config --cflags`
	USBLIBS = `libusb-config --libs`
	EXE_SUFFIX =
	OSFLAG = -D LINUX
else
	USBFLAGS = -I \include
	USBLIBS = -L \lib -lusb
	EXE_SUFFIX = .exe
	OSFLAG = -D WIN
endif

LIBS    = $(USBLIBS)
INCLUDE = library
CFLAGS  = $(USBFLAGS) $(LIBS) -I$(INCLUDE) -O -g $(OSFLAG)

LWLIBS = opendevice littleWire littleWire_util littleWire_servo 
EXAMPLES = pic24f adc blink blink_ws2812 rgb_cycle_ws2812 fade_ws2812 button servo i2c_blinkM spi_LTC1448 onewire softPWM hardwarePWM debugConsole lwbuttond

.PHONY:	clean library docs

all: library $(EXAMPLES)

library: $(LWLIBS)

$(LWLIBS):
	@echo Building library: $@...
	$(CC) $(CFLAGS) -c library/$@.c 

$(EXAMPLES): $(addsuffix .o, $(LWLIBS))
	@echo Building example: $@...
	$(CC) $(CFLAGS) -o $@$(EXE_SUFFIX) examples/$@.c $^ $(LIBS)

docs:
	doxygen ./docs/doxygen.conf
	cd ./docs/latex/; make all

clean:
	rm -rf $(EXAMPLES)$(EXE_SUFFIX) *.o *.exe *.dSYM docs/html docs/latex
