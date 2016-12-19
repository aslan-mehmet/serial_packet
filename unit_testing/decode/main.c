#include "sp.h"
#include <stdio.h>
#include <stdint.h>

/* float value:0.123456 addr:2
 * ./a.out 96 1 4 80 d6 fc 3d 2 0 95 96 2
 *
 * uint32:0x12345678 addr:1
 * ./a.out 96 1 4 78 56 34 12 1 0 9 96 2
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

	sp_thread();

	sp_deinit();
	
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
        case 1:
                u32 = *((uint32_t *) vptr);
                printf("u32:0x%x", u32);
                break;
        case 2:         /* float */
                f = *((float *) vptr);
                printf("float:%f", f);
                break;
        default:
                sp_error(SPE_UNDEF_ADDR);
        }

        printf("\n");

}

void sp_error(uint8_t n)
{
        printf("err code:%d\n", n);
}
