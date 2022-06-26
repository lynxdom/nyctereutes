#pragma once

#include "../defines.h"

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

#if RELEASE_BUILD == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

typedef enum log_level
{
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE
} log_level;

API uint8_t initialize_logging();
API void shutdown_logging();

void log_output( log_level level_, const char * msg_, ... );
void console_output( log_level level_, const char * msg_, ... );

#define LOG_FATAL( message, ... ) log_output( LOG_LEVEL_FATAL, message, ##__VA_ARGS__)
#define LOG_ERROR( message, ... ) log_output( LOG_LEVEL_ERROR, message,  ##__VA_ARGS__)

#if LOG_WARN_ENABLED == 1
    #define LOG_WARN( message, ... ) log_output( LOG_LEVEL_WARN, message,  ##__VA_ARGS__)
#else
    #define LOG_WARN( message, ... )
#endif

#if LOG_INFO_ENABLED == 1
    #define LOG_INFO( message, ... ) log_output( LOG_LEVEL_INFO, message,  ##__VA_ARGS__)
    #define CONSOLE_INFO( message, ... ) console_output( LOG_LEVEL_INFO, message,  ##__VA_ARGS__ )
#else
    #define LOG_INFO( message, ... )
    #define CONSOLE_INFO( message, ... )
#endif

#if LOG_DEBUG_ENABLED == 1
    #define LOG_DEBUG( message, ... ) log_output( LOG_LEVEL_DEBUG, message,  ##__VA_ARGS__)
#else
    #define LOG_DEBUG( message, ... )
#endif

#if LOG_TRACE_ENABLED == 1
    #define LOG_TRACE( message, ... ) log_output( LOG_LEVEL_TRACE, message,  ##__VA_ARGS__)
    #define CONSOLE_TRACE( message, ... ) console_output( LOG_LEVEL_TRACE, message,  ##__VA_ARGS__ )
#else
    #define LOG_DEBUG( message, ... )
    #define CONSOLE_TRACE( message, ... )
#endif