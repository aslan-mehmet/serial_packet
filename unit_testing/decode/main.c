#include "serial.h"
#include <stdio.h>
int main(void)
{
	serial_init();
	uint8_t arr[] = {0x5c, 0x01, 0x32, 0x04, 0x78, 0x56, 0x34, 0x12, 0x08, 0x5c, 0x02};

	for (int i = 0; i < 11; ++i) {
		serial_receives_byte(arr[i]);
	}
	
	while (1) {
		serial_loop();
	}
	return 0;
}

void serial_payload_handler(uint8_t payload_id, uint8_t payload_size, void *payload)
{
	uint32_t data;
	safe_memory_copy(&data, payload, payload_size);

	printf("pid:%x psize:%x p:%x\n", payload_id, payload_size, data);
}
