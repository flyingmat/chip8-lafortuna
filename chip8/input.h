#ifndef CHIP8_INPUT_H
#define CHIP8_INPUT_H

#define ROTA	PE4
#define ROTB	PE5
#define SWC		PE7
#define SWN		PC2
#define SWE		PC3
#define SWS		PC4
#define SWW		PC5

enum state {
    MENU,
    GAME
};

enum keyset {
    KS0123,
    KS4567,
    KS89AB,
    KSCDEF
};

extern volatile enum state c8state;

extern volatile enum keyset ks;
extern volatile uint16_t input_mask;
extern volatile uint16_t c8input;
extern volatile short k;

void init_input();
void process_input();
void scan_encoder();

#endif
