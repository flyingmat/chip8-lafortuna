#include <string.h>
#include <stdlib.h>
#include "ff.h"
#include "lcd.h"
#include "ui.h"

#define MAX_FILES 32

char** files;
unsigned short f_amount;
unsigned short menu_i;

void draw_menu(unsigned short i) {
    display.x = 6;
    display.y = 6;

    if (i != menu_i) {
        rectangle b = {2, display.width-3, 6 + menu_i * 8, 6 + menu_i * 8 + 7};
        fill_rectangle(b, BLACK);
        menu_i = i;
    }

    unsigned short c = 0;
    while (c < f_amount) {
        if (c == i) {
            rectangle l = {2, display.width-3, display.y, display.y + 7};
            fill_rectangle(l, WHITE);
            display.foreground = BLACK;
            display.background = WHITE;
        } else {
            display.foreground = WHITE;
            display.background = BLACK;
        }

        display_string(files[c]);

        display.x = 6;
        display.y += 8;

        c++;
    }
}

void init_menu() {
    files = malloc(sizeof(char*) * MAX_FILES);
    f_amount = 0;
    menu_i = 0;

    DIR dir;
    f_opendir(&dir, "");
    FILINFO f;
    do {
        f_readdir(&dir, &f);
        if (f.fname[0] == '\0')
            break;

        files[f_amount] = malloc(13 * sizeof(char));
        memcpy(files[f_amount], f.fname, 13 * sizeof(char));

        f_amount++;
    } while (f_amount < MAX_FILES);

    rectangle w = {0, display.width-1, 0, display.height-1};
    fill_rectangle(w, WHITE);
    rectangle b = {2, display.width-3, 2, display.height-3};
    fill_rectangle(b, BLACK);
    draw_menu(0);
}
