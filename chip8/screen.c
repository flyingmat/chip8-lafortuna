#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "screen.h"
#include "lcd.h"
#include "cpu.h"

volatile uint8_t* prev;
extern volatile uint8_t* memory;

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
            if ((memory[dbii] >> (7 - dbk)) & 0x01)// && !((prev[dbii - 0xF00] >> (7 - dbk)) & 0x01))
                fill_rectangle(r, WHITE);
            else// if (!((memory[dbii] >> (7 - dbk)) & 0x01) && (prev[dbii - 0xF00] >> (7 - dbk)) & 0x01)
                fill_rectangle(r, BLACK);
        }
        //prev[dbii - 0xF00] = memory[dbii];
    }
}
