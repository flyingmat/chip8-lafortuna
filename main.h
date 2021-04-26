enum state{MENU,GAME};

#define ROTA	PE4
#define ROTB	PE5
#define SWC		PE7
#define SWN		PC2
#define SWE		PC3
#define SWS		PC4
#define SWW		PC5

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ff.h"
#include "lcd.h"
#include "ui.h"
#include "cpu.h"

FATFS fs;
