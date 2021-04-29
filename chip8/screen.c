#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lcd.h"
#include "cpu.h"
#include "input.h"
#include "screen.h"

void draw_screen() {
    for (uint16_t dbii = 0xF00; dbii <= 0xFFF; dbii++) {
        for (uint8_t dbk = 0; dbk < 8; dbk++) {
            display.x = 0;
            display.y = 0;
            unsigned char x = 4 * (((dbii - 0xF00) * 8 + dbk) % 64);
            unsigned char y = 4 * (((dbii - 0xF00) * 8 + dbk) / 64);
            rectangle r = {x, x+3, y, y+3};
            if ((memory[dbii] >> (7 - dbk)) & 0x01)
                fill_rectangle(r, WHITE);
            else
                fill_rectangle(r, BLACK);
        }
    }

    // display.foreground = WHITE;
    // display.background = BLACK;
    // char bbb[16];
    // sprintf(bbb, "%04x", memory[pc] << 8 | memory[pc+1]);
    // display.x = 280;
    // display.y = 20;
    // display_string(bbb);
    // for (int tt = 0; tt < 16; tt++) {
    //     sprintf(bbb, "%02x: %02x", tt, v[tt]);
    //     display.x = 280;
    //     display.y += 10;
    //     display_string(bbb);
    // }

    display.x = 40;
    display.y = 220;
    display.foreground = WHITE;
    display.background = BLACK;
    uint16_t inc = c8input;
    if (ks == KS0123) {
        if (inc & 0x8) {
            display.foreground = BLACK;
            display.background = WHITE;
        } else {
            display.foreground = WHITE;
            display.background = BLACK;
        }
        display_string("3");

        display.x += 10;
        if (inc & 0x4) {
            display.foreground = BLACK;
            display.background = WHITE;
        } else {
            display.foreground = WHITE;
            display.background = BLACK;
        }
        display_string("2");

        display.x += 10;
        if (inc & 0x2) {
            display.foreground = BLACK;
            display.background = WHITE;
        } else {
            display.foreground = WHITE;
            display.background = BLACK;
        }
        display_string("1");

        display.x += 10;
        if (inc & 0x1) {
            display.foreground = BLACK;
            display.background = WHITE;
        } else {
            display.foreground = WHITE;
            display.background = BLACK;
        }
        display_string("0");
    }
    else if (ks == KS4567) {
        inc >>= 4;
        if (inc & 0x8) {
            display.foreground = BLACK;
            display.background = WHITE;
        } else {
            display.foreground = WHITE;
            display.background = BLACK;
        }
        display_string("7");

        display.x += 10;
        if (inc & 0x4) {
            display.foreground = BLACK;
            display.background = WHITE;
        } else {
            display.foreground = WHITE;
            display.background = BLACK;
        }
        display_string("6");

        display.x += 10;
        if (inc & 0x2) {
            display.foreground = BLACK;
            display.background = WHITE;
        } else {
            display.foreground = WHITE;
            display.background = BLACK;
        }
        display_string("5");

        display.x += 10;
        if (inc & 0x1) {
            display.foreground = BLACK;
            display.background = WHITE;
        } else {
            display.foreground = WHITE;
            display.background = BLACK;
        }
        display_string("4");
    }
    else if (ks == KS89AB) {
        inc >>= 8;
        if (inc & 0x8) {
            display.foreground = BLACK;
            display.background = WHITE;
        } else {
            display.foreground = WHITE;
            display.background = BLACK;
        }
        display_string("B");

        display.x += 10;
        if (inc & 0x4) {
            display.foreground = BLACK;
            display.background = WHITE;
        } else {
            display.foreground = WHITE;
            display.background = BLACK;
        }
        display_string("A");

        display.x += 10;
        if (inc & 0x2) {
            display.foreground = BLACK;
            display.background = WHITE;
        } else {
            display.foreground = WHITE;
            display.background = BLACK;
        }
        display_string("9");

        display.x += 10;
        if (inc & 0x1) {
            display.foreground = BLACK;
            display.background = WHITE;
        } else {
            display.foreground = WHITE;
            display.background = BLACK;
        }
        display_string("8");
    }
    else {
        inc >>= 12;
        if (inc & 0x8) {
            display.foreground = BLACK;
            display.background = WHITE;
        } else {
            display.foreground = WHITE;
            display.background = BLACK;
        }
        display_string("F");

        display.x += 10;
        if (inc & 0x4) {
            display.foreground = BLACK;
            display.background = WHITE;
        } else {
            display.foreground = WHITE;
            display.background = BLACK;
        }
        display_string("E");

        display.x += 10;
        if (inc & 0x2) {
            display.foreground = BLACK;
            display.background = WHITE;
        } else {
            display.foreground = WHITE;
            display.background = BLACK;
        }
        display_string("D");

        display.x += 10;
        if (inc & 0x1) {
            display.foreground = BLACK;
            display.background = WHITE;
        } else {
            display.foreground = WHITE;
            display.background = BLACK;
        }
        display_string("C");
    }
}
