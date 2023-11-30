#include "defines.h"
#include "bo_logic.h"
#include "bo_result.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define CHECK_SUCCESS(X, MSG)                   \
    do                                          \
    {                                           \
        BO_Result r = X;                        \
        if (r != BO_SUCCESS)                    \
        {                                       \
            printf("%s [error: %i]\n", MSG, r); \
            exit(1);                            \
        }                                       \
    } while (false)

bool BO_check_collision(const BO_Entity *entity1, const BO_Entity *entity2)
{

    return (
        (entity1->rectangle.position.x < entity2->rectangle.position.x + entity2->rectangle.width) &&
        (entity1->rectangle.position.x + entity1->rectangle.width > entity2->rectangle.position.x) &&
        (entity1->rectangle.position.y < entity2->rectangle.position.y + entity2->rectangle.height) &&
        (entity1->rectangle.height + entity1->rectangle.position.y > entity2->rectangle.position.y));
}

BO_Result BO_update_player(BO_Entity *player, BO_Vector2D* velocity)
{
    BO_vector2D_add(&player->rectangle.position, velocity);

    velocity->y += BO_Gravity;

    if (player->rectangle.position.y > BO_Ground_level - player->rectangle.height)
    {
        player->rectangle.position.y = BO_Ground_level - player->rectangle.height;
        velocity->y *= 0.0f;
    }

    if (player->rectangle.position.x < 0.0f || player->rectangle.position.x > BO_Window_w)
    {
        velocity->x *= 0.0f;
    }
    return BO_SUCCESS;
}