#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "cpu.h"
#include "ff.h"

uint8_t* memory;
uint16_t* stack;
uint8_t* v;
uint16_t i;
uint16_t pc;
uint8_t sp;

void init_cpu() {
    memory = malloc(4096 * sizeof(uint8_t));
    memset(memory, 0, 4096 * sizeof(uint8_t));
    stack = malloc(16 * sizeof(uint16_t));
    memset(stack, 0, 16 * sizeof(uint16_t));
    v = malloc(16 * sizeof(uint8_t));
    memset(v, 0, 16 * sizeof(uint8_t));
    i = 0;
    pc = 0x200;
    sp = 0;
}

void load_game(char* fname) {
    FIL f;
    f_open(&f, fname, 'r');
    unsigned int br;
    f_read(&f, &memory[200], f.fsize, &br);
}

void cycle_cpu() {
    uint16_t inst = memory[pc] << 8 | memory[pc+1];
    uint8_t x = (inst & 0x0F00) >> 8, y = (inst & 0x00F0) >> 4;
    switch (inst >> 12) {
        case 0x0:
            switch(inst & 0x000F) {
                case 0x0:
                    break;
                case 0xE:
                    pc = stack[--sp];
                    break;
            }
            break;
        case 0x1:    // 1nnn - JP addr
            pc = inst & 0x0FFF;
            break;
        case 0x2:    // 2nnn - CALL addr
            stack[sp++] = pc;
            pc = inst & 0x0FFF;
            break;
        case 0x3:    // 3xkk - SE Vx, byte
            if (v[x] == (inst & 0x00FF))
                pc++;
            break;
        case 0x4:    // 4xkk - SNE Vx, byte
            if (v[x] != (inst & 0x00FF))
                pc++;
            break;
        case 0x5:    // 5xy0 - SE Vx, Vy
            if (v[x] == v[y])
                pc++;
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
                pc++;
            break;
        case 0xA:    // Annn - LD I, addr
            i = inst & 0x0FFF;
            break;
        case 0xB:    // Bnnn - JP V0, addr
            pc = (inst & 0x0FFF) + v[0x0];
            break;
        case 0xC:    // Cxkk - RND Vx, byte
            v[x] = rand() % 256 & (inst & 0x00FF);
            break;
        case 0xD:    // Dxyn - DRW Vx, Vy, nibble
            break;

    }
    pc++;
}
