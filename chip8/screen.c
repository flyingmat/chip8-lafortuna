#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "screen.h"
#include "lcd.h"
#include "cpu.h"

volatile uint8_t* prev;
extern volatile uint8_t* memory;
extern volatile uint8_t* v;
extern volatile uint16_t pc;

void init_screen() {
    prev = malloc(0xFF * sizeof(uint8_t));
    vmemset(prev, 0, 0xFF * sizeof(uint8_t));
}

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

    char bbb[16];
    sprintf(bbb, "%04x", memory[pc]);
    display.x = 280;
    display.y = 20;
    display_string(bbb);
    for (int tt = 0; tt < 16; tt++) {
        sprintf(bbb, "%02x: %02x", tt, v[tt]);
        display.x = 280;
        display.y += 10;
        display_string(bbb);
    }
}
