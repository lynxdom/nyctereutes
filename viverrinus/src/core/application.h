#pragma once

#include "../defines.h"

namespace Application
{
    typedef struct app_config
    {
        int16_t start_pos_x;
        int16_t start_pos_y;
        int16_t start_width;
        int16_t start_height;

        const char * name;
    }app_config;

    API bool application_create( app_config *config_ );
    API bool application_run();
}