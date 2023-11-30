#ifndef BO_LOGIC_H
#define BO_LOGIC_H

#include "bo_rectangle.h"
#include "bo_result.h"
#include "bo_list.h"
#include "bo_vector2d.h"

#include <stdint.h>

typedef struct BO_Entity
{
    BO_Rectangle rectangle;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    int hp;
} BO_Entity;


bool BO_check_collision(const BO_Entity *entity1, const BO_Entity *entity2);

BO_Result BO_update_player(BO_Entity *player, BO_Vector2D* velocity);


#endif // BO_LOGIC_H