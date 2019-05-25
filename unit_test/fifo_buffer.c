#include <stdint.h>
#include "../fifo.h"

int main(void)
{
        uint8_t buf[10];
        fifo my_fifo;

        fifo_init(&my_fifo, buf, sizeof(buf));

        for (int i = 1; i <= 100; ++i) {
                int ret = fifo_write(&my_fifo, (uint8_t) i);
                if (i > 9 && ret == 0) // wrote non existing buffer location
                        return 1;
        }

        for (int i = 1; i <= 100; ++i) {
                int read = fifo_read(&my_fifo);

                if (i <= 9 && read != i) // read something different than what we put
                        return 2;

                if (i > 9 && read != -1 ) // read valid data at non existing buffer location
                        return 3;
        }

        return 0;
}
