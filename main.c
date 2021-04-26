#include "main.h"

volatile int8_t delta;
volatile enum state c8state = MENU;

int8_t enc_delta();

void init() {
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;

    DDRE &= ~_BV(PE5) & ~_BV(PE4);
	PORTE |= _BV(PE5) | _BV(PE4);

    /* Ensure all pins are inputs with pull-ups enabled */
	DDRE &= ~_BV(ROTA) & ~_BV(ROTB) & ~_BV(SWC);
	PORTE |= _BV(ROTA) | _BV(ROTB) | _BV(SWC);
	DDRC &= ~_BV(SWN) & ~_BV(SWE) & ~_BV(SWS) & ~_BV(SWW);
	PORTC |= _BV(SWN) | _BV(SWE) | _BV(SWS) | _BV(SWW);
	/* Configure interrupt for any edge on rotary and falling edge for button */
	EICRB |= _BV(ISC40) | _BV(ISC50) | _BV(ISC70);

    EIMSK |= _BV(INT4) | _BV(INT5) | _BV(INT7);

	TCCR0A = _BV(WGM01);
	TCCR0B = _BV(CS01)
               | _BV(CS00);   /* F_CPU / 64 */



    /* SET OCR0A FOR A 2 MS PERIOD / 500 Hz */
	OCR0A = 250;    // 250 * 0.000008 = 0.002s

    /* ENABLE TIMER INTERRUPT */
	TIMSK0 |= _BV(OCIE0A);

    init_lcd();
    set_frame_rate_hz(61);
    EIMSK |= _BV(INT6);

    f_mount(&fs, "", 0);
    init_menu();

    init_cpu();
}

volatile short k = 0;

int main() {

    init();

    sei();

    while (1) {
        if (c8state == MENU) {
            k -= enc_delta();
            if (k < 0)
                k = 0;
            else if (k >= f_amount)
                k = f_amount - 1;
        }
    }

    return 0;
}

ISR(TIMER0_COMPA_vect) {
    cli();
    if (c8state == GAME) {
        cycle_cpu();
    }
    sei();
}

ISR(INT6_vect) {
    if (c8state == MENU) {
        draw_menu(k);
    } else {
        //draw_screen();
    }
}

ISR(INT7_vect) {
    //load_game(files[menu_i]);
    if (c8state == MENU) {
        cli();
        k = 0;
        load_game(files[menu_i]);

        sei();
    }
}

ISR(INT4_vect) {
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
}

ISR(INT5_vect, ISR_ALIASOF(INT4_vect));

int8_t enc_delta() {
    int8_t val;

    cli();
    val = delta;
    delta &= 1;
    sei();

    return val >> 1;
}