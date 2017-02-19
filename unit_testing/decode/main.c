#include "sp.h"
#include <stdio.h>
#include <stdint.h>

/* float value:9.8 addr:1
 * ./a.out 96 1 4 cd cc 1c 41 1 0 5d 96 2
 */
int main(int argc, char **argv)
{
	uint32_t u;
	
	if (sp_init())
                while (1)
                        ;

	for (int i = 1; i < argc; ++i) {
		sscanf(argv[i], "%x", &u);

		sp_frx_put(u);
	}

	sp_decode();
	
	return 0;
}

int sp_tx_send(uint8_t *ptr, uint8_t len)
{
}

void sp_handler(void *vptr, uint16_t addr)
{
        float f;
        uint32_t u32;

        printf("addr:%d ", addr);

        switch (addr) {
        case 1:         /* float */
                f = *((float *) vptr);
                printf("float:%f", f);
                break;
        default:
                sp_error(SPE_UNDEF_ADDR);
        }

        printf("\n");

	sp_reg &= ~sp_rx_lock;

}

void sp_error(uint8_t n)
{
        printf("err code:%d\n", n);
}
