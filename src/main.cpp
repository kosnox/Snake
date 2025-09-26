#include <cstdio>
#include <string.h>
#include <stdlib.h>

#include "terminal/terminal.h"
#include "terminal_in_out/terminal_impl_stdio.h"
#include "game/gamelogic.h"

#define MAX_X 80
#define MAX_Y 25

void create_window();
void print_frame(uint32_t x, uint32_t y);


int main()
{
    terminal_init_t init_struct;
    init_struct.rd_func = std_terminal_read;
    init_struct.rd_func_arg = NULL;
    init_struct.wr_func = std_terminal_write;
    init_struct.wr_func_arg = NULL;
    std_terminal_initialize();

    int32_t max_x = 80;
    int32_t max_y = 25;

    terminal_init(&init_struct);

    terminal_clearScreen();

    terminal_setColor(TC_CYAN, TC_DEFAULT);
    terminal_showCursor(false);

    create_window();

    terminal_setColor(TC_GREEN, TC_DEFAULT);
    int32_t posX = 2, posY = 2;
    bool quit = false;
    int32_t retVal = 0;
    while (!quit) {
        if (!retVal) {
            terminal_setColor(TC_GREEN, TC_DEFAULT);
            terminal_setCursorPos(posX, posY);
            terminal_printf("*");
            int key = terminal_getKey();
            int32_t newPosX = posX;
            int32_t newPosY = posY;

            quit = move_object(key, &newPosX, &newPosY);
            retVal = collision_wall(newPosX, newPosY, 1, 1, MAX_X, MAX_Y);
            //if (newPosY <= 1) newPosY = 2;
            //if (newPosY >= MAX_Y - 1) newPosY = MAX_Y - 1;
            //if (newPosX <= 1) newPosX = 2;
            //if (newPosX >= MAX_X - 1) newPosX = MAX_X - 1;

            terminal_setCursorPos(posX, posY);
            terminal_setColor(TC_DEFAULT, TC_DEFAULT);
            terminal_printf(" ");
            posX = newPosX;
            posY = newPosY;
        }
    }
    terminal_resetStyle();
    terminal_clearScreen();
    return 0;
}


void create_window() {
    uint32_t idx_map = 0;
    int32_t pos_x = 2;
    int32_t pos_y = 2;

    terminal_setCursorPos(pos_x, pos_y);
    terminal_setColor(TC_DEFAULT, TC_DEFAULT);
    terminal_setColor(TC_CYAN, TC_DEFAULT);

    print_frame(1, 1);
}

void print_frame(uint32_t x, uint32_t y)
{
    uint32_t pos_x = x;
    uint32_t pos_y = y;
    terminal_setColor(TC_GREEN, TC_DEFAULT);
    for (int32_t i = 1; i <= MAX_X; i++) {
        terminal_setCursorPos(i, 1);
        terminal_printf("X");
        terminal_setCursorPos(i, MAX_Y);
        terminal_printf("X");
    }
    for (int32_t i = 1; i <= MAX_Y; i++) {
        terminal_setCursorPos(1, i);
        terminal_printf("X");
        terminal_setCursorPos(MAX_X, i);
        terminal_printf("X");
    }
    terminal_setColor(TC_DEFAULT, TC_DEFAULT);
}
