#include <avr/io.h>
#include <avr/interrupt.h>
#include "ui.h"
#include "lcd.h"
#include "cpu.h"
#include "input.h"

volatile enum state c8state = MENU;

volatile enum keyset ks = KS0123;
volatile uint16_t input_mask = 0x0000;
volatile uint16_t c8input = 0x0000;
volatile int8_t delta = 0;
volatile short k = 0;

int8_t enc_delta();

void init_input() {
	DDRE &= ~_BV(ROTA) & ~_BV(ROTB) & ~_BV(SWC);
	PORTE |= _BV(ROTA) | _BV(ROTB) | _BV(SWC);
	DDRC &= ~_BV(SWN) & ~_BV(SWE) & ~_BV(SWS) & ~_BV(SWW);
	PORTC |= _BV(SWN) | _BV(SWE) | _BV(SWS) | _BV(SWW);

	EICRB |= /*_BV(ISC40) | _BV(ISC50) |*/ _BV(ISC70);

    EIMSK |= /*_BV(INT4) | _BV(INT5) |*/ _BV(INT7);
}

void process_input() {
	c8input = 0x0000;

	if (!(PINC & _BV(SWW)))
		c8input |= 1;
	if (!(PINC & _BV(SWN)))
		c8input |= 1 << 1;
	if (!(PINC & _BV(SWE)))
		c8input |= 1 << 2;
	if (!(PINC & _BV(SWS)))
		c8input |= 1 << 3;

	c8input <<= (4 * (uint8_t) ks);
}

void scan_encoder() {
    static int8_t last;
    int8_t new, diff;
    uint8_t wheel;


    /*
       Scan rotary encoder
       ===================
       This is adapted from Peter Dannegger's code available at:
       http://www.mikrocontroller.net/articles/Drehgeber
    */

    wheel = PINE;
    new = 0;
    if( wheel  & _BV(PE4) ) new = 3;
    if( wheel  & _BV(PE5) )
    new ^= 1;		       	/* convert gray to binary */
    diff = last - new;
    if( diff & 1 ){			/* bit 0 = value (1) */
        last = new;
        delta += (diff & 2) - 1;	/* bit 1 = direction (+/-) */
    }

	k -= enc_delta();

	if (c8state == MENU) {
	    if (k < 0)
	        k = 0;
	    else if (k >= f_amount)
	        k = f_amount - 1;
	} else {
		k %= 4;
		if (k < 0)
			k = 4 + k;
		ks = (enum keyset) (k);
	}

}

// ISR(INT5_vect, ISR_ALIASOF(INT4_vect));

ISR(INT7_vect) {
	if (c8state == MENU) {
		cli();
		k = 0;
        display.foreground = WHITE;
        display.background = BLACK;
        clear_screen();
        load_game(files[menu_i]);
        c8state = GAME;
		sei();
	}
}

int8_t enc_delta() {
    int8_t val;

    cli();
    val = delta;
    delta &= 1;
    sei();

    return val >> 1;
}
