#include "sp.h"
#include <stdio.h>

int main(void)
{
	if (sp_init())
		while (1)
			;


	float f = 9.8;
	
	sp_encode(&f, FLOAT, 1);

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
