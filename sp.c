/*
 * author: Mehmet ASLAN
 * date: December 10, 2016
 * rev: 0.1
 * no warranty, no licence agreement
 * use it at your own risk
 */

#include "sp.h"
#include "fifo.h"
#include <stdlib.h>

/* escape sequences */
#define ESC_SP 0x96		/* sp escape byte */
#define ESC_START 1
#define ESC_STOP 2
#define ESC_PAR 3		/* parity */
#define ESC_SELF 4		/* itself */

/* private functions */
int sp_encode_init(void);
void sp_encode_deinit(void);

int sp_decode_init(void);
void sp_decode_deinit(void);

/* handle bytes in fifo rx non-blocking */
void sp_decode(void);

/* global variables */
uint8_t sp_reg = 0;

/* private variables */
static uint8_t txi = 0;
static uint8_t *tx;

static uint8_t *rx;
static uint8_t rxi = 0;
static uint8_t last_get = 0;

static fifo *pfrx;

int sp_init(void)
{
	if(sp_encode_init())
                return -1;
	if(sp_decode_init())
                return -2;

	return 0;
}

void sp_deinit(void)
{
	sp_encode_deinit();
	sp_decode_deinit();
}

/* all func must work in non-blocking mode */
void sp_thread(void)
{
	sp_decode();
}

void spacket_init(uint8_t data_type, uint16_t packet_addr, spacket *p)
{
        p->vsize = data_type;
        p->addr = packet_addr;
}

/*
 * ENCODING PART
 */

int sp_encode_init(void)
{
	tx = (uint8_t *) malloc(sizeof(char) * MPL);

	if (tx == NULL)
		return -1;

	tx[0] = ESC_SP;
	tx[1] = ESC_START;

	return 0;
}

void sp_encode_deinit(void)
{
	free(tx);
}

/* marks escape sequence in packet content with ESC_SELF
 * can fill all buffer
 * check at the end is there enough space to put stop seq
 */
static void put_byte(uint8_t b)
{
        if (txi == MPL)
                return;

	tx[txi++] = b;

	if (b == ESC_SP && txi != MPL)
              tx[txi++] = ESC_SELF;  
}

int sp_encode(void *vptr, spacket packet)
{
	if (sp_reg & sp_tx_lock)
		return -1;

	uint8_t cs = 0;		/* checksum */
	uint8_t *ptr = (uint8_t *) vptr;

	txi = 2;

	put_byte(packet.vsize);

	for (int i = 0; i < packet.vsize; ++i) {
		cs ^= ptr[i];

		put_byte(ptr[i]);
	}

	uint8_t *tp = (uint8_t *) (&packet.addr);

	for (int i = 0; i < 2; ++i) {
		cs ^= tp[i];

		put_byte(tp[i]);
	}

	put_byte(cs);

	if (txi > (MPL -2)) {
		/* dont have space to put stop seq */
		sp_error(SPE_TX_FULL);
		return -2;
	}
	
	tx[txi++] = ESC_SP;
	tx[txi++] = ESC_STOP;

	sp_reg |= sp_tx_lock;

        sp_tx_send(tx, txi);

	return 0;
}

/* 
 * DECODING PART
 */


int sp_decode_init(void)
{
	rx = (uint8_t *) malloc(sizeof(char) * PCL);

	if (rx == NULL)
		return -1;

	static fifo frx;
	pfrx = &frx;
	create_fifo(pfrx, FRXS);

	if (pfrx == NULL)
		return -1;

	return 0;
}

void sp_decode_deinit(void)
{
	free(rx);
	free_fifo(pfrx);
}

/* special chracters for decode
 * negative values represents received escape seq
 */
#define NEG_NO -1		/* no data, fifo empty */
#define NEG_NEED -2		/* esc_sp received */
#define NEG_START -3
#define NEG_STOP -4
#define NEG_PAR -5
#define NEG_UNDEF -6

static int get_byte(void)
{
	int16_t v = read_fifo(pfrx);
	int16_t r;

	if (v < 0)
		return NEG_NO;

	if (last_get == ESC_SP) {
		switch (v) {
		case ESC_START:
			r = NEG_START;
			break;
		case ESC_STOP:
			r = NEG_STOP;
			break;
		case ESC_PAR:
			r = NEG_PAR;
			break;
		case ESC_SELF:
			r = ESC_SP;
			break;
		default:
			r = NEG_UNDEF;
		}

	} else {
		if (v == ESC_SP)
			r = NEG_NEED;
		else
			r = v;
	}

	last_get = v;
	return r;
}

/* their locations do not change relative to the end of packet */
#define vsize rx[0]
#define vcs rx[rxi-1]
#define vpam rx[rxi-2]
#define vpal rx[rxi-3]

static void process_rx(void)
{
	uint8_t *p = (uint8_t *) malloc(sizeof(uint8_t) * vsize);
	uint16_t addr;
	uint8_t *paddr = (uint8_t *) (&addr);
	uint8_t cs = 0;

	if (p == NULL)
		return;
	
	/* check if packet right size */
	if (rxi != vsize + 4) {
		sp_error(SPE_LEN);
		return;
	}

	for (int i = 0; i < vsize; ++i) {
		cs ^= rx[i+1];

		p[i] = rx[i+1];
	}

	cs ^= vpal;
	cs ^= vpam;

	if (cs != vcs) {
		sp_error(SPE_CS);
                return;	
        }

	paddr[0] = vpal;
	paddr[1] = vpam;

        sp_error(SPE_SUCCESS);

        sp_reg |= sp_rx_lock;
	sp_handler(p, addr);

        free(p);
}

void sp_decode(void)
{
	if (sp_reg & sp_rx_lock)
		return;

	int r;

	while ((r = get_byte()) != NEG_NO) {
		switch (r) {
		case NEG_NEED:
			break;
		case NEG_START:
			sp_reg |= sp_start_rcvd;
			rxi = 0;
			break;
		case NEG_STOP:
			if (sp_reg & sp_start_rcvd)
                                process_rx();
			
			sp_reg &= ~sp_start_rcvd;
			break;
		case NEG_PAR:
			sp_error(SPE_PAR);
			break;
                case NEG_UNDEF:
                        sp_error(SPE_UNDEF_SEQ);
                        break;
		default:
			if (r < 0) /* for not to forget all negs */
				while (1)
					;
			if (sp_reg & sp_start_rcvd) {
				if (rxi == PCL)
					sp_error(SPE_RX_FULL);
				else
					rx[rxi++] = (uint8_t) r;
			}
		}
	}
}

void sp_frx_put(uint8_t v)
{
        if (write_fifo(pfrx,  v))
		sp_error(SPE_FRX_FULL);
}
