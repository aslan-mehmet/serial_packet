#include "fifo.h"
#include <stdio.h>

int main(void)
{
        fifo f;
        init_fifo(&f, 50);

        int i;
        for (i = 1; i != 71; ++i)
                if (write_fifo(&f, i))
                        break;

        printf("could not write %d\n", i);

        while ((i = read_fifo(&f)) >= 0)
                printf("%d ", i);
        
        return 0;
}
