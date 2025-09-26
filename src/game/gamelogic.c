#include "gamelogic.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>



int32_t move_object(uint32_t keyboard, int32_t *pos_x, int32_t *pos_y)
{
    switch (keyboard) {
    case 's':
        *pos_y += 1;
        break;
    case 'a':
        *pos_x -= 1;
        break;
    case 'd':
        *pos_x += 1;
        break;
    case 'w':
        *pos_y -= 1;
        break;
    case 'q':
        return 1;
        break;
    }
    return 0;
}


int32_t collision_wall(uint32_t pos_x, uint32_t pos_y, uint32_t min_x, uint32_t min_y, uint32_t max_x, uint32_t max_y)
{
	if (pos_x <= min_x) return 1;
	if (pos_y <= min_y) return 1;
	if (pos_x >= max_x) return 1;
	if (pos_y >= max_y) return 1;

	return 0;
}

