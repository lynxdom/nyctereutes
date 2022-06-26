#include "application.h"
#include "../platform/platform.h"

namespace Application
{
    typedef struct application_state
    {
        bool is_running = false;
        bool is_suspended = false; 
        Platform::platform_state platform;
        int16_t width;
        int16_t height;
        std::chrono::steady_clock::duration last_time;
    }application_state;

    static bool initialized = false;
    static application_state app_state = {};

    API bool application_create( app_config *config_ )
    {
        if( initialized )
        {
            LOG_ERROR("application_called called more than once.");
            return false;
        }

        initialize_logging();

        LOG_FATAL("Test value %d", 1);
        LOG_ERROR("Test value %d", 2);
        LOG_WARN("Test value %f", 3.14);
        LOG_INFO("Test value %s", "test String");
        LOG_DEBUG("Test Value %d", 4);
        LOG_TRACE("Test Value %d", 5);

        app_state.is_running = true;

        if(!Platform::platform_system_startup( &app_state.platform, 
                                               config_->name, 
                                               config_->start_pos_x,
                                               config_->start_pos_y, 
                                               config_->start_width,
                                               config_->start_height) )
        {
            return false;
        }

        initialized = true;

        return 0;
    }

    API bool application_run()
    {
        while( app_state.is_running )
        {
            if( !Platform::platform_pump_messages() )
            {
                app_state.is_running = false;
            }
        }

        Platform::platform_system_shutdown( );

        shutdown_logging();

        return 0;
    }
}