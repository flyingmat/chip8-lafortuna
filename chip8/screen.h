#ifndef CHIP8_SCREEN_H
#define CHIP8_SCREEN_H

extern volatile uint8_t* prev;

void init_screen();
void draw_screen();

#endif
