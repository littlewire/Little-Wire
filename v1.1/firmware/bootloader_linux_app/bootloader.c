/**
 * Bootloader um dem Mikrocontroller Bootloader von Peter Dannegger anzusteuern
 * Teile des Codes sind vom original Booloader von Peter Dannegger (danni@alice-dsl.net)
 *
 * @author Andreas Butti
 * Bug fixes by Ilya G. Goldberg
 *   - flashsize allocation based on free user flash
 *   - serial comm speed set in both directions
 *   - lastaddr initialized correctly
 *   - in linux (and ANSI C) clock() is CPU time, not elapsed time.  Changed to gettimeofday().
 *   - no busy-polling of read
 *   - checking write() to avoid hardware buffer overruns
 *   - enabled hardware buffers for a ~100x speedup.
 */


/// Includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h> // for readlink()

#include "com.h"
#include "protocol.h"

/// Prototypes
void usage();
void connect_device();
void sendcommand(unsigned char c);
int check_crc();
int read_info();
long readval();
int flash(char verify, const char * filename);
int readhex(FILE *fp, unsigned long *addr, unsigned char *data);
int sscanhex(char *str, unsigned int *hexout, int n);
char * readHexfile(const char * filename, int flashsize, unsigned long * lastaddr);

/// Definitions
// These are in milliseconds (it is not likely that the timer resolution is better than 10 ms)
#define TIMEOUT	  300 // 0.3s
#define TIMEOUTP 4000 // 4s


///show elapsed time @ ms / s
///only ONE definition!
//#define SHOW_TIME_MS
#define SHOW_TIME_S


/// Attributes
int buffersize;
long flashsize;

static struct timeval t_start; //time
static int foo;
// Text file with device signatures
// Find it path based on binary path.
char devices_file_p[256]="", *chp;
#define DEVICES_FILE "devices.txt"

/**
 * Main, startup
 */
int main(int argc, char *argv[]) {

	// Filename of the HEX File
	const char * hexfile = NULL;

	// 1 if verify, 2 if programm
	char verify = 0;

	// Serial device
	const char * device = "/dev/ttyS0";

	// Baudrate
	int baud = 4800;
	int baudid = -1;

	// if crc is supported (not supportet if 2)
	int crc_on;
	
	// The path to the binary (for finding devices.txt)
	readlink("/proc/self/exe", devices_file_p, 255); // symlink to this binary
	chp = strrchr (devices_file_p,'/');  // get the last path separator
	if (chp) strcpy (chp+1,DEVICES_FILE); // copy the device filename after the path separator


	// print header
	printf("\n");
	printf("=================================================\n");
	printf("|             BOOTLOADER, Target: V2.1          |\n");
	printf("=================================================\n");

	// Parsing / checking parameter
	int i;
	int type = 0;

	for(i = 1; i < argc; i++) {
		if(*argv[i] == '-') {
			type = argv[i][1];
		}
		else {
			switch(type) {
				case 'd':
					device = argv[i];
					break;
				case 'b':
					baud = atoi(argv[i]);
					break;
				case 'v':
					verify = 1;
					hexfile = argv[i];
					break;
				case 'p':
					verify = 2;
					hexfile = argv[i];
					break;
				default:
					printf("Wrong parameter!\n");
					usage();
			}
			type = 0;
		}
	}

	if(hexfile == NULL) {
		printf("No hexfile specified!\n");
		usage();
	}

	if(verify == 0) {
		printf("No Verify / Programm specified!\n");
		usage();
	}

	// Checking baudrate
	for(i = 0; i < BAUD_CNT; i++) {
		if (baud_value[i] == baud) {
			baudid = i;
			break;
		}
	}

	if(baudid == -1) {
		printf("Unknown baudrate (%i)!\n", baud);
		usage();
	}

	printf("Device   : %s\n", device);
	printf("Baudrate : %i\n", baud);
	printf("%s: %s\n", (verify == 1 ? "Verify   " : "Program  "), hexfile);
	printf("-------------------------------------------------\n");

	if(!com_open(device, baud_const[baudid])) {
		printf("Open com port failed!\n");
		exit(2);
	}

	connect_device();
	crc_on = check_crc();
	read_info();
	
	/*if(read_info()) {
	}
	else {
		printf("Reading device information failed!\n");
	}*/

	if(crc_on != 2) {
		crc_on = check_crc();
		switch(crc_on) {
			case 2:
				printf("No CRC support.\n");
				break;
			case 0:
				printf("CRC enabled and OK.\n");
				break;
			case 3:
				printf("CRC check failed!\n");
				break;
			default:
				printf("Checking CRC Error (%i)!\n", crc_on);
				break;
		}
	}
	else {
		printf("No CRC support.\n");
	}

	flash(verify==1, hexfile);
	if( crc_on != 2 ) {
		if( check_crc() )
			printf( "CRC-Error !\n");
		else
			printf("CRC: o.k.\n");
	}

#ifdef SHOW_TIME_MS	 
	//time @ ms
	printf("Elapsed time: %d s\n", elapsed_msecs (&t_start));
#endif

#ifdef SHOW_TIME_S	
	printf("Elapsed time: %.3f seconds\n", elapsed_secs (&t_start));
#endif

	printf("...starting application\n\n");
	sendcommand(START);//start application
	sendcommand(START);

	com_close();//close opened com port
	return 0;
}

/**
 * Read a hexfile
 */
char * readHexfile(const char * filename, int flashsize, unsigned long * lastaddr) {
	char * data = malloc(flashsize);
	FILE *fp;
	int len;
	unsigned char line[512];
	unsigned long addr = 0;

	if(data == NULL) {
		printf("Memory allocation error!\n");
		return NULL;
	}

	memset( data, 0xFF, flashsize );

	if(NULL == (fp = fopen(filename, "r"))){
		printf("File \"%s\" open failed!\n\n", filename);
		free(data);
		exit(0);
	}

	printf("Reading %s... ", filename);


	// reading file to "data"
	while((len = readhex(fp, &addr, line)) >= 0) {
		if(len) {
			if( addr + len > flashsize ) {
				fclose(fp);
				free(data);
				printf("Hex-file too large for target!\n");
				return NULL;
			}
			memcpy(data+addr, line, len);
			addr += len;

			if(*lastaddr < addr-1) {
				*lastaddr = addr-1;
			}
 // This doesn't appear to do anything, and probably incorrect due to
 // addr += len; above, introducing an off-by-one error
			addr++;
		}
	}

	fclose(fp);

	printf("File read.\n");
	return data;
}


/**
 * Flashes or verify the controller
 */
int flash(char verify, const char * filename) {
	char * data = NULL;
	int i;
	int msg;
	unsigned char d1;
	unsigned long addr;
	unsigned long lastaddr=0;

	//read data from hex-file
	data = readHexfile(filename, flashsize, &lastaddr);

	if(data == NULL) {
		printf("Reading file failed.\n");
		return 0;
	}
	printf("Highest address in hex file: 0x%05lX (%lu Bytes)\n",lastaddr,lastaddr);

	// Sending commands to MC
	if(verify == 0){
		printf("Writing program memory...\n");
		printf("Programming \"%s\": 00000 - 00000", filename);
		sendcommand(PROGRAM);
	} 
	else {
		sendcommand(VERIFY);
		printf("Verifying program memory...\n");
		
		foo = com_getc(TIMEOUT);
		if(foo == BADCOMMAND) {
			printf("Verify not available\n");
			return 0;
		}
		printf( "Verify %s: 00000 - 00000", filename);
	}

	// Sending data to MC
	for(i = buffersize, addr = 0;; addr++) {
		switch(d1 = data[addr]){
			case ESCAPE:
			case 0x13:
				com_putc(ESCAPE);
				d1 += ESC_SHIFT;
			default:
				com_putc(d1);
		}

		if(--i == 0) {
			printf( "\b\b\b\b\b%05lX", addr + 1);
			fflush(stdout);
			if(!verify) {
				msg = 0;
				msg = com_getc(TIMEOUTP);
				if (msg != CONTINUE) {
					printf(" failed. Did not receive CONTINUE despite more data to write. Got 0x%04X (%u)\n",(unsigned)((unsigned char)msg),(unsigned)((unsigned char)msg));
					free(data);
					return 0;
				}
			}
			i = buffersize;
		}
		
		if(addr == lastaddr) {
			com_putc(ESCAPE);
			com_putc(ESC_SHIFT); // A5,80 = End

			printf("\b\b\b\b\b%05lX", addr);
			msg = 0;
			msg = com_getc(TIMEOUTP);
			if (msg != SUCCESS) {
				printf(" failed. Did not receive SUCCESS at end of programming. Got 0x%04X (%u)\n",(unsigned)((unsigned char)msg),(unsigned)((unsigned char)msg));
				free(data);
				return 0;
			} else {
				printf(" SUCCESS\n");
			}
			break;
		}
	}
	printf("\n");

	free(data);

	return 1;
}//int flash(char verify, const char * filename)



/**
 * Reads the hex file
 *
 * @return 1 to 255 number of bytes, -1 file end, -2 error or no HEX File
 */
int readhex(FILE *fp, unsigned long *addr, unsigned char *data) {
	char hexline[524]; // intel hex: max 255 byte
	char * hp = hexline;
	unsigned int byte;
	int i;
	unsigned int num;
	unsigned int low_addr;

	if(fgets( hexline, 524, fp ) == NULL) {
		return -1; // end of file
	}

	if(*hp++ != ':') {
		return -2; // no hex record
	}

	if(sscanhex(hp, &num, 2)) {
		return -2; // no hex number
	}

	hp += 2;

	if(sscanhex(hp, &low_addr, 4)) {
		return -2;
	}

	*addr &= 0xF0000L;
	*addr += low_addr;
	hp += 4;

	if(sscanhex( hp, &byte, 2)) {
		return -2;
	}

	if(byte == 2) {
		hp += 2;
		if(sscanhex(hp, &low_addr, 4)) {
			return -2;
		}
		*addr = low_addr * 16L;
		return 0; // segment record
	}

	if(byte == 1) {
		return 0; // end record
	}

	if(byte != 0) {
		return -2; // error, unknown record
	}

	for(i = num; i--;) {
		hp += 2;
		if(sscanhex(hp, &byte, 2)) {
			return -2;
		}
		*data++ = byte;
	}
	return num;
}

/**
 * reads hex data from string
 */
int sscanhex(char *str, unsigned int *hexout, int n) {
	unsigned int hex = 0, x = 0;

	for(; n; n--) {
		x = *str;
		if(x >= 'a') {
			x += 10 - 'a';
		} else if(x >= 'A') {
			x += 10 - 'A';
		} else {
			x -= '0';
		}

		if(x >= 16) {
			break;
		}

		hex = hex * 16 + x;
		str++;
	}
	
	*hexout = hex;
	return n; // 0 if all digits read
}


/**
 * prints usage
 */
void usage() {
	printf("./booloader [-d /dev/ttyS0] [-b 9600] -[v|p] file.hex\n");
	printf("-d Device\n");
	printf("-b Baudrate\n");
	printf("-v Verify\n");
	printf("-p Programm\n\n");
	printf("Author: Andreas Butti (andreasbutti@bluewin.ch)\n");

	exit(1);
}

/**
 * Try to connect a device
 */
void connect_device() {
	const char * ANIM_CHARS = "-\\|/";
	const char PASSWORD[6] = {'P', 'e', 'd', 'a', 0xff, 0};
	
	int state = 0;
	int in = 0;
	
	printf("Waiting for device... Press CTRL+C to exit.  ");
	
	while(1) {
		printf("\b%c", ANIM_CHARS[state]);
		fflush(stdout);

		com_puts(PASSWORD);
		in = com_getc(0);

		if(in == CONNECT) {
			sendcommand(COMMAND);

			// Empty buffer
			while(1) {
				switch(com_getc(TIMEOUT)){
					case SUCCESS:
						printf("\n...Connected!\n");
						gettimeofday(&t_start, NULL);
						return;
					case -1:
						printf("\n...Connection timeout!\n\n");
						exit (0);
				}
			}
		}
		state++;
		state = state % 4;
		usleep(10000);//wait 10ms
	}
}//void connect_device()

/**
 * Sending a command
 */
void sendcommand(unsigned char c) {
  com_putc(COMMAND);
  com_putc(c);
}

/**
 * Checking CRC Support
 *
 * @return 2 if no crc support, 0 if crc supported, 1 fail, exit on timeout
 */
int check_crc() {
	int i;
	unsigned int crc1;

	sendcommand(CHECK_CRC);
	crc1 = crc;
	com_putc(crc1);
	com_putc(crc1 >> 8);

	i = com_getc(TIMEOUT);
	switch (i) {
		case SUCCESS:
			return 0;
		case BADCOMMAND:
			return 2;
		case FAIL:
			return 1;
		case -1:
			printf("...Device does not answer!\n\n");
			exit (0);
		default:
			return i;
	}
}

/**
 * prints the device signature
 *
 * @return true on success; exit on error
 */
int read_info() {
	long i, j;
	char s[256];
	FILE *fp;

///get bootloader REVISON
	sendcommand(REVISION);
	
	i = readval();
	if(i == -2) {
		printf("Bootloader Version unknonwn (Fail)\n");
	} else {
		printf("Bootloader    : V%lX.%lX\n", i>>8, i&0xFF);
	}

///get SIGNATURE
	sendcommand(SIGNATURE);
	
	i = readval();
	if (i == -2) {
		printf("Reading device SIGNATURE failed!\n\n");
		exit (0);
	}

	if((fp = fopen(devices_file_p, "r")) != NULL) {
		while(fgets(s, 256, fp)) {
			if(sscanf(s, "%lX : %s", &j, s) == 2){ // valid entry
				if(i == j) {
					break;
				}
			}
			*s = 0;
		}
		fclose(fp);
	} 
	else {
		sscanf ("(?)" , "%s", s);
		printf("File \"%s\" not found!\n",DEVICES_FILE);
	}

	if (!*s) sprintf (s,"Device not in \"%s\"",DEVICES_FILE);

	printf("Target        : %06lX %s\n", i, s);

///get BUFFSIZE
	sendcommand(BUFFSIZE);

	i = readval();
	if (i == -2) {
		printf("Reading BUFFSIZE failed!\n\n");
		exit (0);
	}
	buffersize = i;
	printf("Buffer        : %6ld Bytes\n", i );

///get USERFLASH
	sendcommand(USERFLASH);

	i = readval();
	if (i == -2) {
		printf("Reading FLASHSIZE failed!\n\n");
		exit (0);
	}
	if( i > MAXFLASH) {
		printf("Device an flashsize do not match!\n");
		exit (0);
	}
	
	flashsize = i;
	printf("Size available: %6ld Bytes\n\n", i );
	
	return 1;
}//int read_info()

/**
 * Reads a value from bootloader
 *
 * @return value; -2 on error; exit on timeout
 */
long readval() {
	int i;
	int j = 257;
	long val = 0;

	while(1) {
		i = com_getc(TIMEOUT);
		if(i == -1) {
			printf("...Device does not answer!\n");
			exit(0);
		}

		switch(j) {
			case 1:
				if(i == SUCCESS) {
					return val;
				}
				break;

			case 2:
				val = val * 256 + i;
				j= 1;
				break;

			case 3:
				val = val * 256 + i;
				j= 2;
				break;

			case 4:
				val = val * 256 + i;
				j= 3;
				break;

			case 256:
				j = i;
				break;

			case 257:
				if(i == FAIL) {
					return -2;
				}

				else if(i == ANSWER) {
					j = 256;
				}
				break;

			default:
				printf("\nError: readval, i = %i, j = %i, val = %li\n", i, j, val);
				return -2;
		}
	}
}


/* end of file */
