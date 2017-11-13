#include "serial.h"
#include <stdio.h>

int main(void)
{
	serial_init();

	uint32_t u32 = 0x12345678;
	serial_encode(0x32, sizeof(u32), &u32);

	printf("expected\n");
	printf("5c 01 32 04 78 56 34 12 cs 5c 02\n");  
	while (1) {
		serial_loop();
	}
		
	return 0;
}

void serial_send_byte(uint8_t byt)
{
	printf("%x\n", byt);
}
