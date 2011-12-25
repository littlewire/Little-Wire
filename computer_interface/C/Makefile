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

LIBS    = $(USBLIBS) -lpthread
INCLUDE = library
CFLAGS  = $(USBFLAGS) $(LIBS) -I$(INCLUDE) -O -g $(OSFLAG)

EXAMPLES = adc blink button

.PHONY:	clean library

all: library $(EXAMPLES)

library:
	@echo Building libraries...
	$(CC) $(CFLAGS) -c library/littleWire.c 
	$(CC) $(CFLAGS) -c library/opendevice.c 

$(EXAMPLES): littleWire.o opendevice.o
	@echo Building example:$(@)...
	$(CC) $(CFLAGS) -o $@$(EXE_SUFFIX) examples/$@.c $^

clean:
	rm -f $(EXAMPLES) *.o *.exe
