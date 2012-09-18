/**
 * Bootloader um dem Mikrocontroller Bootloader von Peter Dannegger anzusteuern
 *
 * License: GPL
 *
 * @author Andreas Butti
 */


/// Includes
#include <time.h>
#include <string.h>

#include "com.h"


/// Attributes

// Device
int fd;
// Old settings
struct termios oldtio;
// CRC checksum
unsigned int crc = 0;

/// Prototypes
void calc_crc(unsigned char d);


speed_t baud_const[BAUD_CNT] = { B50, B75, B110, B134, B150, B200, B300, B600,
		B1200, B1800, B2400, B4800, B9600, B19200, B38400, B57600, B115200 };

unsigned long baud_value[BAUD_CNT] = { 50, 75, 110, 134, 150, 200, 300, 600,
		1200, 1800, 2400, 4800, 9600, 19200, 38400, 57600, 115200 };


/**
 * Opens com port
 *
 * @return true if successfull
 */
char com_open(const char * device, speed_t baud) {
	struct termios newtio;

	// Open the device
	fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd < 0) {
		return 0;
	}

	// Save old settings
	tcgetattr(fd, &oldtio);

	// Init memory
	memset(&newtio, 0x00 , sizeof(newtio));
	cfmakeraw(&newtio);

	// settings flags
	newtio.c_cflag |= CS8;
	newtio.c_cflag |= CLOCAL;
	newtio.c_cflag |= CREAD;
	newtio.c_iflag = IGNPAR | IGNBRK;
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;


	// Timeout in 100ms
	newtio.c_cc[VTIME] = 0;
	// read 1 character
	newtio.c_cc[VMIN] = 0;

	// Setting baudrate
	cfsetispeed (&newtio, baud);
	cfsetospeed (&newtio, baud);

	// Flushing buffer
	tcflush(fd, TCIOFLUSH);

	// aplying new configuration
	tcsetattr(fd, TCSANOW, &newtio);

	return 1;
}


/**
 * Close com port and restore settings
 */
void com_close() {
	// restore old settings
	tcsetattr(fd, TCSANOW, &oldtio);

	// close device
	close(fd);
}

/**
 * Sends one char
 */
void com_putc(unsigned char c) {

// We do this in a loop to make sure we don't everrun the buffer.
	while ( write(fd, &c, 1) != 1 ) {
		usleep (1000);
	}
	calc_crc( c ); // calculate transmit CRC
}

/**
 * Recives one char or -1 if timeout
 */
int com_getc(int timeout) {
	char c;
	struct timeval t_now;

	com_flush();
	gettimeofday(&t_now, NULL);
	while ( read(fd, &c, 1) != 1) {
		// take a little nap (1 ms, but probably more like 10 due to scheduler)
		usleep (1000);
		if (elapsed_msecs (&t_now) >= timeout) {
			return -1;
		}
	}
	return (unsigned char)c;
}

/**
 * Flushes the buffer
 */
void com_flush() {
	// tcflush deletes the buffer, so we use tcdrain(fd)
	tcdrain(fd);
}

/**
 * Sends a string
 */
void com_puts(const char *text) {

	while(*text) {
		com_flush(); // this prevents write() from waiting.
		com_putc( *text++);
	}
}

/**
 * Calculate the new CRC sum
 */
void calc_crc(unsigned char d) {
	int i;

	crc ^= d;
	for( i = 8; i; i-- ){
		crc = (crc >> 1) ^ ((crc & 1) ? 0xA001 : 0 );
	}
}


double elapsed_secs (struct timeval *start) {
double secs;
long int ssecs=start->tv_sec, susecs=start->tv_usec;
struct timeval t_now;

	gettimeofday(&t_now, NULL);

	/* Perform the carry for the later subtraction by updating y. */
	if (t_now.tv_usec < susecs) {
		int nsec = (susecs - t_now.tv_usec) / 1000000 + 1;
		susecs -= 1000000 * nsec;
		ssecs += nsec;
	}
	if (t_now.tv_usec - susecs > 1000000) {
		int nsec = (t_now.tv_usec - susecs) / 1000000;
		susecs += 1000000 * nsec;
		ssecs -= nsec;
	}

	secs = t_now.tv_sec - ssecs;
	secs += (double)(t_now.tv_usec - susecs) / 1000000.0;
	
	return secs;
	}

long elapsed_msecs (struct timeval *start) {
long int msecs, ssecs=start->tv_sec, susecs=start->tv_usec;
struct timeval t_now;

	gettimeofday(&t_now, NULL);

	/* Perform the carry for the later subtraction by updating y. */
	if (t_now.tv_usec < susecs) {
		int nsec = (susecs - t_now.tv_usec) / 1000000 + 1;
		susecs -= 1000000 * nsec;
		ssecs += nsec;
	}
	if (t_now.tv_usec - susecs > 1000000) {
		int nsec = (t_now.tv_usec - susecs) / 1000000;
		susecs += 1000000 * nsec;
		ssecs -= nsec;
	}

	msecs = (t_now.tv_sec - ssecs) * 1000;
	msecs += (t_now.tv_usec - susecs) / 1000;
	return msecs;
	}


