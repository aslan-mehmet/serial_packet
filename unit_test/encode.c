#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "../serial_packet.h"

uint8_t encoded_data[9];

int main(void)
{
        uint16_t payload = 0x1234;
	uint8_t payload_id = 0x16;
	uint8_t *payload_array = (uint8_t *) (&payload);
	uint8_t payload_size = (uint8_t) sizeof(payload);
	uint8_t checksum = payload_array[0] ^ payload_array[1]; 

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
        serial_packet_encode(payload_id, payload_size, &payload);
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
