#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lcd.h"
#include "cpu.h"
#include "input.h"
#include "screen.h"

// void draw_ks(keyset kss) {
//     char str[4];
//
//     switch(kss) {
//         case KS0123:
//             str = "0123";
//             break;
//         case KS4567:
//             str = "4567";
//             break;
//         case KS89AB:
//             str = "89AB";
//             break;
//         case KSCDEF:
//             str = "CDEF";
//             break;
//     }
//
//     if (ks == kss) {
//         uint16_t inc = c8input >> (4 * kss);
//         for (uint8_t i = 0; i < 4; i++) {
//
//         }
//     } else {
//
//     }
// }

void draw_screen() {
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
