#include "serial_packet.h"
#include <stdio.h>
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
 
void print(uint8_t payload_size, void *payload)
{
	static int cnt = 0;

	uint8_t *p = (uint8_t *) payload;

	for (int i = 0; i < payload_size; ++i) {
		printf("%d:%x\n", ++cnt, p[i]);
	}
}

void serial_packet_handler(uint8_t payload_id, uint8_t payload_size
			    , void *payload)
{
        union VARS var;

        if (sizeof(var) >= payload_size) {
                safe_memory_copy(&var.u8, payload, payload_size);
        }

	switch (payload_id) {
	case 1:
		print(payload_size, payload);
		break;
	}
}
