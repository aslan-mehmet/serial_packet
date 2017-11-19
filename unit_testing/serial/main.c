#include "serial.h"
#include <stdio.h>


uint32_t u32;
int32_t i32;
float f;
double d;


int main(void)
{
	serial_init();

	i32 = -100;
	serial_encode(0x01, sizeof(i32), &i32);

	f = -248.75;
	serial_encode(0x02, sizeof(f), &f);

	i32 = 0;
	f = 0;
	
	while (1) {
		serial_loop();
	}
		
	return 0;
}

void serial_send_byte(uint8_t byt)
{
	serial_receives_byte(byt);
}

void serial_payload_handler(uint8_t payload_id, uint8_t payload_size, void *payload)
{
	switch (payload_id) {
	case 0x01:
		safe_memory_copy(&i32, payload, payload_size);
		printf("i32:%d\n", i32);
		break;
	case 0x02:
		safe_memory_copy(&f, payload, payload_size);
		printf("f:%f\n", f);
		break;
	}
}
