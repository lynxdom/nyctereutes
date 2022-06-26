#pragma once

#include "core/application.h"
#include <functor>


typedef struct game
{
    Application::app_config app_config;

    bool (*inititalize)(struct game* game_inst);
    bool (*update)(struct game* game_inst, uint32_t delta_time);
    bool (*render)(struct game* game_inst, uint32_t delta_time);
    void (*on_resize)(string game*, uint32_t width, uint32_t height);

    void* state;
}