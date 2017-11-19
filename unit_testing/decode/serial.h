#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdint.h>
#define SERIAL_RX_BUF_SIZE 60
#define SERIAL_PAYLOAD_BUF_SIZE 60
#define CONTENT_BUF_SIZE 60

void serial_init(void);
void serial_loop(void);
void serial_receives_byte(uint8_t byt);
void serial_payload_handler(uint8_t payload_id, uint8_t payload_size
			    , void *payload) __attribute__ ((weak));
void safe_memory_copy(void *dest, void *src, uint8_t size);
#endif	/* __SERIAL_H */
