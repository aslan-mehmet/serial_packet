#include "serial_packet.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv)
{
        if (serial_packet_init()) {
		return 1;
	}

	int num = 0;
	for (int i = 1; i < argc; ++i) {
		sscanf(argv[i], "%d", &num);
		serial_packet_read((uint8_t)num);
	} 
        

	time_t hold_time = time(NULL);
        
        while (time(NULL) - hold_time < 3) {
                serial_packet_flush();
        }
        
        return 0;
}

void serial_packet_print(uint8_t byt)
{
	printf("%x ", byt);
}
