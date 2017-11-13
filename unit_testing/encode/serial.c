#include "serial.h"
#include "fifo.h"

#define SERIAL_ESCAPE_BYTE ((uint8_t) 0x5c)
#define SERIAL_START ((uint8_t) 0x01)
#define SERIAL_STOP ((uint8_t) 0x02)
/* if serial_escape_byte occured in data content, state not mean to escaped */
#define SERIAL_NOT_ESCAPE ((uint8_t) 0x03)

static fifo tx_fifo;
static uint8_t tx_buf[SERIAL_TX_BUF_SIZE];

void serial_init(void)
{
	tx_fifo.buf = tx_buf;
	tx_fifo.size = SERIAL_TX_BUF_SIZE;
	init_fifo(&tx_fifo);
}

static void data_write(uint8_t data)
{
	write_fifo(&tx_fifo, data);
	
	if (data == SERIAL_ESCAPE_BYTE) {
		write_fifo(&tx_fifo, SERIAL_NOT_ESCAPE);
	}
}

static void escaped_write(uint8_t data)
{
	write_fifo(&tx_fifo, SERIAL_ESCAPE_BYTE);
	write_fifo(&tx_fifo, data);
}

void serial_encode(uint8_t pid, uint8_t psize, void *pptr)
{
        escaped_write(SERIAL_START);

	data_write(pid);

	data_write(psize);

	uint8_t *payload = (uint8_t *) pptr;
	uint8_t checksum = 0;
	
	for (uint16_t i = 0; i < psize; ++i) {
		checksum ^= payload[i];
		data_write(payload[i]);
	}

	data_write(checksum);

	escaped_write(SERIAL_STOP);
}

void serial_loop(void)
{
	int16_t hold;
	
	if ((hold = read_fifo(&tx_fifo)) != -1) {
		serial_send_byte((uint8_t) hold);
	}
}
