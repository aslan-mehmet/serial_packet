#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "../serial_packet.h"

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
        serial_packet_encode(0x16, 2, &payload);
        serial_packet_flush_tx_buf();

        return 0;
}

void serial_packet_print(uint8_t byt)
{
        static int counter = 0;
        if (counter == 9)
                exit(1);

        if (encoded_data[counter] != byt)
                exit(2);
        counter++;
}
