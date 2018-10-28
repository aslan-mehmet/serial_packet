#include <stdio.h>
#include <stdint.h>

int main(void)
{
        uint16_t u16 = 0x0001;
        uint8_t *u8_ptr = (uint8_t *)(&u16);

        if (u8_ptr[0])
                return 0;
        else
                return 1;

        return 0;
}
