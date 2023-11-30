#include "defines.h"
#include "bo_graphics.h"
#include "bo_window.h"
#include "bo_result.h"
#include "bo_logic.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

/**
 *
 * TODO
 *
 * ADD frame-rate independent phisics
 * ADD levels
 * ADD mouse steerign
 * ADD corner bounces
 *
 *
 */

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

int main(int argc, char *argv[])
{
    // load font
    TTF_Init();
    TTF_Font *Pixel = TTF_OpenFont("../data/fonts/PixelForce.ttf", 24);
    SDL_Color White = {255, 255, 255};

    BO_Entity ground = {.rectangle = BO_Rectangle_create_xy(0.0f, BO_Ground_level, BO_Window_w, BO_Ground_height), .hp = -1, .r = 0x00, .g = 0x00, .b = 0x00};
    BO_Entity player = {.rectangle = BO_Rectangle_create_xy(30.0f, BO_Ground_level - 60.0f, 20.0f, 60.0f), .hp = -1, .r = 0x00, .g = 0x00, .b = 0xff};
    BO_Vector2D player_velocity = BO_Vector2D_create();

    BO_Window *window;
    CHECK_SUCCESS(BO_Window_create(&window), "failed to window");

    BO_List *entities = NULL;
    CHECK_SUCCESS(BO_List_assign(&entities), "failed initialize list");

    CHECK_SUCCESS(BO_List_push_back(entities, &ground), "Failed to initialize ground");
    CHECK_SUCCESS(BO_List_push_back(entities, &player), "Failed to initialize player");

    // load breaking block texture
    SDL_Surface *tmp_surface = IMG_Load("../data/textures/breaking.png");
    SDL_Texture *breaking_texture = SDL_CreateTextureFromSurface(window->sdl_renderer, tmp_surface);
    SDL_FreeSurface(tmp_surface);
    SDL_Rect texture_rect;

    BO_ListItr *itr = NULL;
    CHECK_SUCCESS(BO_List_iterator_assign_list(entities, &itr), "failed to assign itr");

    bool running = true;
    bool keys_to_process = true;

    BO_KeyEvent key_event;
    uint64_t fps_cap = 60;
    uint64_t framerate_cap = 1000 / fps_cap;
    uint64_t points = 0;
    uint64_t game_start = SDL_GetTicks64();

    while (running)
    {
        uint64_t loop_start = SDL_GetTicks64();
        keys_to_process = true;
        while (keys_to_process)
        {

            BO_Result event_resut = BO_Window_get_event(window, &key_event);
            if (event_resut == BO_SUCCESS)
            {
                switch (key_event.key_type)
                {
                case BO_KEY_QUIT:
                    printf("bye\n");
                    running = false;
                    goto end;
                    break;
                case BO_KEY_LEFT:
                    if (key_event.key_state == BO_KEYSTATE_DOWN)
                    {
                        player_velocity.x = -2.0f;
                    }
                    else
                        player_velocity.x = 0.0f;
                    break;
                case BO_KEY_RIGHT:
                    if (key_event.key_state == BO_KEYSTATE_DOWN)
                        player_velocity.x = 2.0f;
                    else
                        player_velocity.x = 0.0f;
                    break;
                case BO_KEY_SPACE:
                    if (key_event.key_state == BO_KEYSTATE_DOWN)
                        player_velocity.y = -5.0f;
                    break;

                default:
                    break;
                }
            }
            else if (event_resut == BO_NO_MORE_EVENTS)
            {
                keys_to_process = false;
            }
            else
            {
                printf("error getting event\n");
                exit(1);
            }
        }

        CHECK_SUCCESS(BO_update_player(&player, &player_velocity), "failed to update player");

        BO_List_iterator_reset(entities, &itr);
        BO_Graphics_pre_render(window);
        while (!BO_List_iterator_at_end(itr))
        {
            BO_Entity *entity = (BO_Entity *)BO_List_iterator_value(itr);
            texture_rect.x = entity->rectangle.position.x;
            texture_rect.y = entity->rectangle.position.y;
            BO_Graphics_display_entity(window, entity, texture_rect, breaking_texture);
            // BO_Graphics_draw_rectangle(window, &entity->rectangle, entity->r, entity->g, entity->b);
            BO_List_iterator_advance(&itr);
        }
        // points calculation
        uint64_t new_points = (loop_start - game_start) / 1000;
        if (new_points != points)
        {
            points = new_points;
        }

        // points rendering
        char str[256];
        sprintf(str, "%lu", points);
        SDL_Surface *surfaceMessage = TTF_RenderText_Solid(Pixel, str, White);
        SDL_Rect Message_rect;                          // create a rect
        Message_rect.x = 300;                           // controls the rect's x coordinate
        Message_rect.y = 0;                             // controls the rect's y coordinte
        Message_rect.w = 33 * ((int)log10(points) + 1); // controls the width of the rect
        Message_rect.h = 48;                            // controls the height of the rect
        SDL_Texture *Message = SDL_CreateTextureFromSurface(window->sdl_renderer, surfaceMessage);
        SDL_FreeSurface(surfaceMessage);
        SDL_RenderCopy(window->sdl_renderer, Message, NULL, &Message_rect);

        BO_Graphics_post_render(window);

        uint64_t delta = SDL_GetTicks() - loop_start;
        if (delta < framerate_cap)
        {
            SDL_Delay(framerate_cap - delta);
        }
    }
end:
    BO_Window_destroy(window);
    return 0;
}
