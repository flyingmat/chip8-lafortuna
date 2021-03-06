#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "cpu.h"
#include "ff.h"
#include "lcd.h"
#include "screen.h"
#include "input.h"

volatile uint8_t* memory;
volatile uint16_t* stack;
volatile uint8_t* v;
volatile uint16_t i;
volatile uint16_t pc;
volatile uint8_t sp;

void vmemset(volatile void *s, char c, size_t n) {
    volatile char* p = s;
    while (n-- > 0)
        *p++ = c;
}

void init_cpu() {
    memory = malloc(4096 * sizeof(uint8_t));
    vmemset(memory, 0, 4096 * sizeof(uint8_t));

    uint8_t c8font [] = { 0xF0, 0x90, 0x90, 0x90, 0xF0,
                          0x20, 0x60, 0x20, 0x20, 0x70,
                          0xF0, 0x10, 0xF0, 0x80, 0xF0,
                          0xF0, 0x10, 0xF0, 0x10, 0xF0,
                          0x90, 0x90, 0xF0, 0x10, 0x10,
                          0xF0, 0x80, 0xF0, 0x10, 0xF0,
                          0xF0, 0x80, 0xF0, 0x90, 0xF0,
                          0xF0, 0x10, 0x20, 0x40, 0x40,
                          0xF0, 0x90, 0xF0, 0x90, 0xF0,
                          0xF0, 0x90, 0xF0, 0x10, 0xF0,
                          0xF0, 0x90, 0xF0, 0x90, 0x90,
                          0xE0, 0x90, 0x90, 0x90, 0xE0,
                          0xF0, 0x80, 0xF0, 0x80, 0xF0,
                          0xF0, 0x80, 0xF0, 0x80, 0x80 };

    for (int i = 0; i < 70; i++)
        memory[i] = c8font[i];

    stack = malloc(16 * sizeof(uint16_t));
    vmemset(stack, 0, 16 * sizeof(uint16_t));
    v = malloc(16 * sizeof(uint8_t));
    vmemset(v, 0, 16 * sizeof(uint8_t));
    i = 0;
    pc = 0x200;
    sp = 0;
}

void load_game(char* fname) {
    FIL f;
    f_open(&f, fname, FA_READ);

    uint8_t* buff = malloc(1500 * sizeof(uint8_t));
    memset(buff, 0, (1500) * sizeof(uint8_t));
    unsigned int br;

    f_read(&f, buff, 1500, &br);

    for (int t = 0; t < 1500; t++)
        memory[0x200 + t] = buff[t];

    free(buff);
}

uint8_t draw_sprite(uint8_t x, uint8_t y, uint8_t h) {
    uint8_t setvf = 0;

    for (uint8_t row = 0; row < h; row++) {
        uint16_t memptr = 0xF00 + ((y + row) * 8) + x / 8;
        uint8_t newmem;
        uint8_t oldmem;

        // x matches a byte array location, easier to calculate
        if (!(x % 8)) {
            oldmem = memory[memptr];
            newmem = oldmem ^ memory[i + row];
            memory[memptr] = newmem;
        } else {
            oldmem = (memory[memptr] << (x % 8)) | (memory[memptr+1] >> (8 - (x % 8)));
            newmem = oldmem ^ memory[i + row];
            memory[memptr] ^= memory[i + row] >> (x % 8);
            memory[memptr+1] ^= memory[i + row] << (8 - (x % 8));
        }

        for (uint8_t k = 0; k < 8; k++) {
            // if a pixel is erased, v[f] will be set to 1
            if (!setvf) {
                if (((oldmem >> k) & 0x01) && !((newmem >> k) & 0x01))
                    setvf = 1;
            }

            // each pixel is enlarged 4 times, because of higher ppi
            uint8_t xs = 4 * (x + k);
            uint8_t ys = 4 * (y + row);

            // (320 - 256) / 2 = 32    ->    center the emulated display
            rectangle r = {xs + 32, xs + 3 + 32, ys, ys + 3};
            if (((oldmem >> (7 - k)) & 0x01) != ((newmem >> (7 - k)) & 0x01))
                fill_rectangle(r, (newmem >> (7 - k)) & 0x01 ? WHITE : BLACK);
        }
    }

    return setvf;
}

void cycle_cpu() {
    process_input();
    draw_keyset();

    uint16_t inst = memory[pc] << 8 | memory[pc+1];
    uint8_t x = (inst & 0x0F00) >> 8, y = (inst & 0x00F0) >> 4;

    switch (inst >> 12) {
        case 0x0:
            switch(inst & 0x000F) {
                case 0x0:
                    for (int t = 0; t <= 0xFF; t++)
                        memory[0xF00 + t] = 0;
                    clear_screen();
                    break;
                case 0xE:
                    pc = stack[--sp];
                    break;
            }
            break;
        case 0x1:    // 1nnn - JP addr
            pc = (inst & 0x0FFF) - 2;
            break;
        case 0x2:    // 2nnn - CALL addr
            stack[sp++] = pc;
            pc = (inst & 0x0FFF) - 2;
            break;
        case 0x3:    // 3xkk - SE Vx, byte
            if (v[x] == (inst & 0x00FF))
                pc += 2;
            break;
        case 0x4:    // 4xkk - SNE Vx, byte
            if (v[x] != (inst & 0x00FF))
                pc += 2;
            break;
        case 0x5:    // 5xy0 - SE Vx, Vy
            if (v[x] == v[y])
                pc += 2;
            break;
        case 0x6:    // 6xkk - LD Vx, byte
            v[x] = inst & 0x00FF;
            break;
        case 0x7:    // 7xkk - ADD Vx, byte
            v[x] += inst & 0x00FF;
            break;
        case 0x8:
            switch (inst & 0x000F) {
                case 0x0:    // 8xy0 - LD Vx, Vy
                    v[x] = v[y];
                    break;
                case 0x1:    // 8xy1 - OR Vx, Vy
                    v[x] |= v[y];
                    break;
                case 0x2:    // 8xy2 - AND Vx, Vy
                    v[x] &= v[y];
                    break;
                case 0x3:    // 8xy3 - XOR Vx, Vy
                    v[x] ^= v[y];
                    break;
                case 0x4:    // 8xy4 - ADD Vx, Vy
                {   uint16_t r = v[x] + v[y];
                    v[0xF] = r > 0xFF ? 1 : 0;
                    v[x] = (uint8_t) (r & 0xFF);
                    break;    }
                case 0x5:    // 8xy5 - SUB Vx, Vy
                    v[0xF] = v[x] > v[y] ? 1 : 0;
                    v[x] -= v[y];
                    break;
                case 0x6:    // 8xy6 - SHR Vx {, Vy}
                    v[0xF] = v[x] & 0x000F;
                    v[x] >>= 1;
                    break;
                case 0x7:    // 8xy7 - SUBN Vx, Vy
                    v[0xF] = v[y] > v[x] ? 1 : 0;
                    v[x] = v[y] - v[x];
                    break;
                case 0xE:    // 8xyE - SHL Vx {, Vy}
                    v[0xF] = v[x] >> 7;
                    v[x] <<= 1;
                    break;
            }
            break;
        case 0x9:    // 9xy0 - SNE Vx, Vy
            if (v[x] != v[y])
                pc += 2;
            break;
        case 0xA:    // Annn - LD I, addr
            i = inst & 0x0FFF;
            break;
        case 0xB:    // Bnnn - JP V0, addr
            pc = (inst & 0x0FFF) + v[0x0] - 2;
            break;
        case 0xC:    // Cxkk - RND Vx, byte
            v[x] = rand() % 256 & (inst & 0x00FF);
            break;
        case 0xD:    // Dxyn - DRW Vx, Vy, nibble
            v[0xF] = draw_sprite(v[x], v[y], inst & 0x000F);
            break;
        case 0xE:
            switch (inst & 0x000F) {
                case 0xE:    // Ex9E - SKP Vx
                    if (c8input & (1 << v[x]))
                        pc += 2;
                    break;
                case 0x1:    // ExA1 - SKNP Vx
                    if (!(c8input & (1 << v[x])))
                        pc += 2;
                    break;
            }
            break;
        case 0xF:
            switch (inst & 0x00FF) {
                case 0x07:    // Fx07 - LD Vx, DT
                    v[x] = delay_timer;
                    break;
                case 0x0A:    // Fx0A - LD Vx, K
                    if (!c8input)
                        pc -= 2;
                    else {
                        for (int t = 0; t < 16; t++) {
                            if (c8input & (1 << t)) {
                                v[x] = 1 << t;
                            }
                        }
                    }
                    break;
                case 0x15:    // Fx15 - LD DT, Vx
                    delay_timer = v[x];
                    break;
                case 0x18:    // Fx18 - LD ST, Vx
                    sound_timer = v[x];
                    break;
                case 0x1E:    // Fx1E - ADD I, Vx
                    i += v[x];
                    break;
                case 0x29:    // Fx29 - LD F, Vx
                    i = v[x] * 5;
                    break;
                case 0x33:    // Fx33 - LD B, Vx
                {   uint8_t vv = v[x];

                    memory[i] = 0;
                    memory[i+1] = 0;
                    memory[i+2] = 0;

                    while (vv > 99) {
                        memory[i]++;
                        vv -= 100;
                    }
                    while (vv > 9) {
                        memory[i+1]++;
                        vv -= 10;
                    }
                    while (vv != 0) {
                        memory[i+2]++;
                        vv -= 1;
                    }

                    break;    }
                case 0x55:    // Fx55 - LD [I], Vx
                    for (int t = 0; t <= x; t++)
                        memory[i+t] = v[t];
                    break;
                case 0x65:    // Fx65 - LD Vx, [I]
                    for (int t = 0; t <= x; t++)
                        v[t] = memory[i+t];
                    break;
            }
            break;
    }
    pc += 2;
}
