/**
 * @file fifo.h
 * @author Mehmet ASLAN
 * @date November 19, 2017
 * @copyright Gnu General Public License Version 3 or Later
 * @brief
 * this is a serial communication protocol\n
 * there 2 different characters : escaped characters and normal characters\n
 * data packaged to send via serial byte communication\n
 * single package character order(size in byte):\n
 * START(2) PAYLOAD_ID(1) PAYLOAD_SIZE(1) PAYLOAD(PAYLOAD_SIZE) CHECKSUM(1) STOP(2)\n
 * START and STOP escaped.\n
 * If escape character used in payload, its marked with an additional byte\n
 */
#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdint.h>
/* 
 * @brief default 60, allowed to change
 * @attention must be bigger than single packet size, else packet always corrupted
 */
#define SERIAL_TX_BUF_SIZE 60

/**
 * @brief
 * first thing to do, inits tx rx buffers
 * @attention
 * rx tx buffers size user settable via macros 
 */
void serial_init(void);
/**
 * @brief packages data w/ given parameters and puts into tx buffer 
 */
void serial_encode(uint8_t payload_id, uint8_t payload_size, void *payload_ptr);
/** 
 * @brief
 * put this into your main loop\n
 * handles received bytes in rx buffer
 * also sends bytes in tx buffer
 */
void serial_loop(void);
/**
 * @brief
 * hardware specific implementation to send single byte
 * @attention
 * user have to implement and for now only polling send supported(wait until byte goes)
 */
void serial_send_byte(uint8_t byt) __attribute__ ((weak));

#endif	/* __SERIAL_H */