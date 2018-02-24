#include "serial_packet.h"
#include <stdio.h>
#include <time.h>

int main(void)
{
        if (serial_packet_init()) {
		return 1;
	}

        uint8_t u8_arr[SERIAL_PACKET_MAX_PAYLOAD_SIZE];

        for (int i = 0; i < SERIAL_PACKET_MAX_PAYLOAD_SIZE; ++i) {
                u8_arr[i] = i;
        }

        serial_packet_encode_poll(0x1, SERIAL_PACKET_MAX_PAYLOAD_SIZE, u8_arr);

        for (int i = 0; i < SERIAL_PACKET_MAX_PAYLOAD_SIZE; ++i) {
                u8_arr[i] = 0x5c;
        }

        for (int i = 0; i < SERIAL_PACKET_MAX_PACKET_COUNT + 1; ++i) {
		serial_packet_encode(0x1, SERIAL_PACKET_MAX_PAYLOAD_SIZE, u8_arr);
	}

	time_t hold_time = time(NULL);
        
        while (time(NULL) - hold_time < 3) {
                serial_packet_flush();
        }
        
        return 0;
}

void serial_packet_print(uint8_t byt)
{
	printf("%d ", (int)byt);
}
