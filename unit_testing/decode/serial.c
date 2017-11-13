#include "serial.h"
#include "fifo.h"

#define SERIAL_ESCAPE_BYTE ((uint8_t) 0x5c)
#define SERIAL_START ((uint8_t) 0x01)
#define SERIAL_STOP ((uint8_t) 0x02)
/* if serial_escape_byte occured in data content, state not mean to escaped */
#define SERIAL_NOT_ESCAPE ((uint8_t) 0x03)

static fifo _rx_fifo;
static uint8_t _rx_buf[SERIAL_RX_BUF_SIZE];

static uint8_t _payload_id, _payload_size;
static uint8_t _payload[SERIAL_PAYLOAD_BUF_SIZE];

static uint8_t _content[200];
static uint16_t _content_iteration = 0;
static uint8_t _content_started = 0;
static uint8_t _escape_byte_received = 0;

void serial_init(void)
{
	_rx_fifo.buf = _rx_buf;
	_rx_fifo.size = SERIAL_RX_BUF_SIZE;
	init_fifo(&_rx_fifo);
}

static void process_serial_content(void)
{
        _payload_id = _content[0];
	_payload_size = _content[1];

	if (_content_iteration != _payload_size + 3) {
		return;
	}

	safe_memory_copy(_payload, _content + 2, _payload_size);

	uint8_t checksum = 0;

	for (uint8_t i = 0; i < _payload_size; ++i) {
		checksum ^= _payload[i];
	}

	if (checksum != _content[_content_iteration - 1]) {
		return;
	}
	
	serial_payload_handler(_payload_id, _payload_size, _payload);
}

static void process_received_byte(uint8_t byt)
{
	if (byt == SERIAL_ESCAPE_BYTE) {
		_escape_byte_received = 1;
		return;
	}

	if (_escape_byte_received) {

		switch (byt) {
		case SERIAL_START:
			_content_started = 1;
			_content_iteration = 0;
			break;
		case SERIAL_STOP:
			process_serial_content();
			_content_started = 0;
			_content_iteration = 0;
			break;
		case SERIAL_NOT_ESCAPE:
			if (_content_started) {
				_content[_content_iteration++] = SERIAL_ESCAPE_BYTE;
			}
			break;
		}

		_escape_byte_received = 0;
		return;
	}

	if (_content_started) {
		_content[_content_iteration++] = byt;
	}
}

void serial_loop(void)
{
	int16_t hold;

	if ((hold = read_fifo(&_rx_fifo)) != -1) {
		process_received_byte((uint8_t) hold);
	}

}

void serial_receives_byte(uint8_t byt)
{
	write_fifo(&_rx_fifo, byt);
}

void safe_memory_copy(void *dest, void *src, uint8_t size)
{
	uint8_t *d, *s;
	d = (uint8_t *) dest;
	s = (uint8_t *) src;

	for (int i = 0; i < size; ++i) {
		d[i] = s[i];
	}
}
