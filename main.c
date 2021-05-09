#include "main.h"

volatile uint8_t delay_timer = 0;
volatile uint8_t sound_timer = 0;

int8_t enc_delta();

void init() {
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;

	TCCR0A = _BV(WGM01);
	TCCR0B = _BV(CS01)
               | _BV(CS00);   /* F_CPU / 64 */

    OCR0A = 250;    // 250 * 0.000008 = 0.002s (500Hz)

	TIMSK0 |= _BV(OCIE0A);

    TCCR1A = _BV(WGM11);
	TCCR1B = _BV(CS12);   /* F_CPU / 256 */

    OCR1A = 521;    // 521 * 0.000032 ~= 0.0167s (60Hz)
	TIMSK1 |= _BV(OCIE1A);

    init_input();

    f_mount(&fs, "", 0);

    init_lcd();
    set_frame_rate_hz(75);

    init_menu();
    init_cpu();
}

int main() {
    init();
    sei();

    while (1) {
        scan_encoder();

        if (c8state == MENU) {
            draw_menu(k);
        } else {
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

ISR(TIMER1_COMPA_vect) {
    cli();
    if (c8state == GAME) {
        if (delay_timer != 0)
            delay_timer--;
        if (sound_timer != 0)
            sound_timer--;
    }
    sei();
}
