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


uint8_t encoded_data[9];

int main(void)
{
        uint16_t payload = 0x1234;

        // start sequence
        encoded_data[0] = SERIAL_PACKET_ESCAPE_BYTE;
        encoded_data[1] = SERIAL_PACKET_START;

        assert(0x16 != SERIAL_PACKET_ESCAPE_BYTE);
        encoded_data[2] = 0x16; // payload id

        assert(2 != SERIAL_PACKET_ESCAPE_BYTE);
        encoded_data[3] = 2; // payload size

        // payload 0x1234
        assert(0x34 != SERIAL_PACKET_ESCAPE_BYTE);
        encoded_data[4] = 0x34;
        assert(0x12 != SERIAL_PACKET_ESCAPE_BYTE);
        encoded_data[5] = 0x12;

        assert((0x12 ^ 0x34) != SERIAL_PACKET_ESCAPE_BYTE);
        encoded_data[6] = (0x12 ^ 0x34); // checksum

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

        if (payload_id != 0x16)
                exit(1);

        if (payload_size != 2)
                exit(2);

        if (var.u16 != 0x1234)
                exit(3);
}
