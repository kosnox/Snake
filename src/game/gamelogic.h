#if !defined(GAMELOGIC_H)
#define GAMELOGIC_H
#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

int32_t move_object(uint32_t keyboard, int32_t* pos_x, int32_t* pos_y);
int32_t collision_wall(uint32_t pos_x, uint32_t pos_y, uint32_t min_x, uint32_t min_y, uint32_t max_x, uint32_t max_y);


#if defined(__cplusplus)
}
#endif

#endif // GAMELOGIC_H
