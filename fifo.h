/**
 * @file fifo.h
 * @author Mehmet ASLAN
 * @date November 12, 2017
 * @copyright Gnu General Public License Version 3 or Later
 * @brief 
 * fifo implementation with circular buffer\n
 * data thought as snake which moves in buffer\n
 * snake cant eat its tail\n
 * but length of snake can be zero that means there is no data
 * @bug
 * when multiple threads read and write at the same time dont know what happen
 */

#ifndef __FIFO_H
#define __FIFO_H

#include <stdint.h>
/**
 * @brief
 * fifo data type for internal operations\n
 * allows to create multiple fifos in same project
 */
typedef struct{
	/**
	 * @brief pointer to staticly created buffer
	 */
	uint8_t *buf;
	/**
	 * @brief 
	 * buffer size\n
	 * actual length of the fifo\n
	 * len = size -1\n
	 * like null terminated string\n
	 * why ? --> cause head index always empty
	 */
	uint16_t size;
	/** @brief dont care about these as user */
	uint16_t head, tail;
}fifo;

/** 
 * @brief create a fifo struct and uint8_t buffer. initialize with this function.
 * @attention this function must be called first. actual length of the buffer is 'sizeof(buf)-1'.
 */
void fifo_init(fifo *f, uint8_t *buf, uint16_t buf_size);
/**
 * @retval -1 if full. data not written 
 * @retval 0 success
 */
int8_t fifo_write(fifo *f, uint8_t data);
/**
 * @retval -1 if empty
 * @retval uint8_t data read
 */
int16_t fifo_read(fifo *f);
/** @brief empty all data in fifo */
void fifo_flush(fifo *f);

#endif	/* __FIFO_H */
