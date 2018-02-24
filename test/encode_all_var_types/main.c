#include "serial_packet.h"
#include <stdio.h>
#include <time.h>

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

int main(void)
{
        if (serial_packet_init()) {
		return 1;
	}

	u8 = 8;
	serial_packet_encode_poll(1, sizeof(u8), &u8);

	u16 = 16;
	serial_packet_encode_poll(2, sizeof(u16), &u16);

	u32 = 32;
	serial_packet_encode_poll(3, sizeof(u32), &u32);

	u64 = 64;
	serial_packet_encode_poll(4, sizeof(u64), &u64);

	i8 = -8;
	serial_packet_encode_poll(5, sizeof(i8), &i8);

	i16 = -16;
	serial_packet_encode_poll(6, sizeof(i16), &i16);

	i32 = -32;
	serial_packet_encode_poll(7, sizeof(i32), &i32);

	i64 = -64;
	serial_packet_encode_poll(8, sizeof(i64), &i64);

	f = 0.1;
	serial_packet_encode_poll(9, sizeof(f), &f);

	d = 0.2;
	serial_packet_encode_poll(10, sizeof(d), &d);

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
