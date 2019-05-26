/**
 * @file fifo.c
 * @author Mehmet ASLAN
 * @date November 12, 2017
 * @copyright Gnu General Public License Version 3 or Later
 * @brief 
 * fifo implementation with circular buffer\n
 * How to Use\n
 * create fifo data type\n
 * create a buffer\n
 * set fifo size and buf\n
 * init w/ fifo_init\n
 * done
 */
#include "fifo.h"

void fifo_init(fifo *f, uint8_t *buf, uint16_t buf_size)
{
	f->buf = buf;
	f->size = buf_size;
	f->head = f->tail = 0;
}

/* this dont lets head to eat(catch) its own tail */
static int8_t is_fifo_full(fifo *f)
{
	int32_t a = f->tail;
	--a;

	if (a < 0) {
		a = f->size - 1;
	}

	/* a = index behind the tail */
	if (a == f->head) { 	/* so head is behind the tail */
		return 1;
	}
	
	return 0;
}

int8_t fifo_write(fifo *f, uint8_t data)
{       
	if (is_fifo_full(f)) {
		return -1;
	}
	
	/* put data to head index and inc */
	f->buf[f->head++] = data;
	/* so head is always empty except while writing */
	if (f->head == f->size) {
		f->head = 0;
	}

	return 0;
}

uint16_t fifo_write_buffer(fifo *f, uint8_t *buf, uint16_t buf_size)
{
	for (uint16_t i = 0; i < buf_size; i++)
		if (fifo_write(f, buf[i]))
			return i;
	return buf_size;
}

/* return neg if empty */
int16_t fifo_read(fifo *f)
{
        /*
	 * as said head empty
	 * if tail equals to it, tails also empty
	 */
	if (f->tail == f->head) {
		return -1;
	}
	
	uint8_t data = f->buf[f->tail++];

	if (f->tail == f->size) {
		f->tail = 0;
	}

	return data;
}

void fifo_flush(fifo *f)
{	
	/* so length of snake 0 */
	f->tail = f->head;
}
