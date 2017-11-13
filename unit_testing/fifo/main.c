#include "fifo.h"
#include <stdio.h>

int main(void)
{
        fifo f;
	uint8_t buf[50];

	f.buf = buf;
	f.size = 50;
	init_fifo(&f);

        int i;
        for (i = 1; i != 71; ++i) {
                if (write_fifo(&f, i)) {
                        break;
		}
	}

	puts("expected 50");
        printf("could not write %d\n", i);

        while ((i = read_fifo(&f)) >= 0) {
                printf("%d ", i);
	}
	putchar('\n');
	
	/* fill it again */
	for (i = 1; i != 71; ++i) {
                if (write_fifo(&f, i)) {
                        break;
		}
	}

	flush_fifo(&f);

	if (read_fifo(&f) == -1) {
		puts("flush works");		
	} else {
		puts("flush not works");
	}
        
        return 0;
}
