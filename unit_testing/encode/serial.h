#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdint.h>
/* 
 * @brief default 60, allowed to change
 * @attention must be bigger than single packet size, else packet always corrupted
 */
#define SERIAL_TX_BUF_SIZE 60

void serial_init(void);

/**
 * @brief packages to send, writes to 
 */
void serial_encode(uint8_t payload_id, uint8_t payload_size, void *payload_ptr);
void serial_loop(void);
void serial_send_byte(uint8_t byt) __attribute__ ((weak));

#endif	/* __SERIAL_H */
