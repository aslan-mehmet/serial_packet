#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include "../serial_packet.h"

union VARS {
        uint8_t u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;
        int8_t i8;
        int16_t i16;
        int32_t i32;
        int64_t i64;
        float f;
        double d;
};

uint16_t p;
uint8_t pid;
uint8_t psize;

int main(void)
{
	uint8_t encoded_data[9];
	uint16_t payload = 0x1234;
	uint8_t payload_id = 0x16;
	uint8_t *payload_array = (uint8_t *) (&payload);
	uint8_t payload_size = (uint8_t) sizeof(payload);
	uint8_t checksum = payload_array[0] ^ payload_array[1];

	p = payload;
	pid = payload_id;
	psize = payload_size; 

        // start sequence
        encoded_data[0] = SERIAL_PACKET_ESCAPE_BYTE;
        encoded_data[1] = SERIAL_PACKET_START;

        assert(payload_id != SERIAL_PACKET_ESCAPE_BYTE);
        encoded_data[2] = payload_id;

        assert(payload_size != SERIAL_PACKET_ESCAPE_BYTE);
        encoded_data[3] = payload_size;

	assert(payload_size == 2);
        assert(payload_array[0] != SERIAL_PACKET_ESCAPE_BYTE);
        encoded_data[4] = payload_array[0];
        assert(payload_array[1] != SERIAL_PACKET_ESCAPE_BYTE);
        encoded_data[5] = payload_array[1];

        assert(checksum != SERIAL_PACKET_ESCAPE_BYTE);
        encoded_data[6] = checksum;

        // stop sequence
        encoded_data[7] = SERIAL_PACKET_ESCAPE_BYTE;
        encoded_data[8] = SERIAL_PACKET_STOP;

        serial_packet_init();

        for (int i = 0; i < 9; ++i)
                serial_packet_read(encoded_data[i]);

        serial_packet_flush_rx_buf();

        return 0;
}

void serial_packet_handler(uint8_t payload_id, uint8_t payload_size
                , void *payload)
{
        union VARS var;

        if (sizeof(var) >= payload_size) {
                safe_memory_copy(&var.u8, payload, payload_size);
        }

        if (payload_id != pid)
                exit(1);

        if (payload_size != psize)
                exit(2);

        if (var.u16 != p)
                exit(3);
}
