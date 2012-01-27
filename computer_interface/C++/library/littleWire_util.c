
#include <littleWire_util.h>


void delay(unsigned int duration)
{
	#ifdef LINUX
		usleep(duration*1000);
	#else
		Sleep(duration);
	#endif
}
