#include "sp.h"
#include <stdio.h>

int main(void)
{
	if (sp_init())
		while (1)
			;

	spacket my_packet;

	spacket_init(UINT32_T, 1, &my_packet);


	uint32_t v = 0x12345678;
	
	sp_encode(&v, my_packet);

        spacket d_packet;
        spacket_init(DOUBLE, 21, &d_packet);
        double d = 0.123456789123;
        sp_encode(&d, d_packet);

	return 0;
}

int sp_tx_send(uint8_t *ptr, uint8_t len)
{
	for (int i = 0; i < len; ++i)
		printf("%x ", ptr[i]);

	printf("\n");
	sp_reg &= ~sp_tx_lock;
}

void sp_handler(void *vptr, uint16_t addr)
{}

void sp_error(uint8_t n)
{}
