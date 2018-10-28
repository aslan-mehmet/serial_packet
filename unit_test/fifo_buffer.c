#include <stdint.h>
#include "../fifo.h"

int main(void)
{
        uint8_t buf[10];
        fifo my_fifo;

        my_fifo.buf = buf;
        my_fifo.size = sizeof(buf);
        init_fifo(&my_fifo);

        for (int i = 1; i <= 100; ++i) {
                int ret = write_fifo(&my_fifo, (uint8_t) i);
                if (i > 9 && ret == 0) // wrote non existing buffer location
                        return 1;
        }

        for (int i = 1; i <= 100; ++i) {
                int read = read_fifo(&my_fifo);

                if (i <= 9 && read != i) // read something different than what we put
                        return 2;

                if (i > 9 && read != -1 ) // read valid data at non existing buffer location
                        return 3;
        }

        return 0;
}