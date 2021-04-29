#ifndef CHIP8_CPU_H
#define CHIP8_CPU_H

#include <stddef.h>

extern volatile uint8_t* memory;
extern volatile uint16_t* stack;
extern volatile uint8_t* v;
extern volatile uint16_t i;
extern volatile uint16_t pc;
extern volatile uint8_t sp;
extern volatile uint8_t delay_timer;
extern volatile uint8_t sound_timer;

void vmemset(volatile void*, char, size_t);
void init_cpu();
void load_game(char*);
void cycle_cpu();

#endif
