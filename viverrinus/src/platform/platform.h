#pragma once

#include "../defines.h"
#include "../core/logger.h"

#include <memory>
#include <chrono>

namespace Platform
{
    typedef struct platform_state
    {
        std::shared_ptr<void> internal_state;
    } platform_state;

    int8_t platform_system_startup(platform_state *platform_state_,
                                   const char *application_name_,
                                   uint32_t x_,
                                   uint32_t y_,
                                   uint32_t width_,
                                   uint32_t height_);

    void platform_system_shutdown();
    int8_t platform_pump_messages();
    std::chrono::steady_clock::duration get_absolute_time();
    void platform_sleep( uint64_t milliseconds_ );

    void * platform_allocate( uint64_t size_, 
                              uint8_t aligned_ );

    void platform_free( void * block_, 
                        uint8_t aligned_ );

    void * platform_zero_memory( void * dest_, 
                                 uint64_t size_ );

    void * platform_copy_memory( void * dest_, 
                                 const void * source_, 
                                 uint64_t size );

    void * platform_set_memory( void *dest, 
                                uint8_t value_, 
                                uint64_t size_ );

    void platform_console_write( const char *message_, 
                                 uint8_t color_);

    void platform_console_write_erro( const char *message_, 
                                      uint8_t color_);

}