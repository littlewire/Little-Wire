# Makefile for Little-Wire

CC=clang
CXX=clang

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
CFLAGS = $(USBFLAGS) $(LIBS) -I$(INCLUDE) -O -Wall -pedantic -g $(OSFLAG)
CXXFLAGS = $(USBFLAGS) $(LIBS) -I$(INCLUDE) -O -Wall -pedantic -g $(OSFLAG)

LWLIBS = opendevice littleWire #littleWire_util littleWire_servo
EXAMPLES = #adc blink button servo

.PHONY:	clean library

all: library $(EXAMPLES)

library: $(LWLIBS)

$(LWLIBS):
	@echo Building libraries...
	$(CXX) $(CXXFLAGS) -c library/littleWire.cpp
	$(CC) $(CFLAGS) -c library/opendevice.c

$(EXAMPLES): $(addsuffix .o, $(LWLIBS))
	@echo Building example: $@...
	$(CXX) $(CXXFLAGS) -o $@$(EXE_SUFFIX) examples/$@.c $^ $(LIBS)

clean:
	rm -f $(EXAMPLES)$(EXE_SUFFIX) *.o *.exe
