#include <fstream>
#include <thread>
#include <mutex>
#include <chrono>
#include <queue>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <filesystem>

#include "logger.h"

#include <stdio.h>
#include <cstdarg>

#define MAX_MESSAGE_SIZE 32000

#define DIRECTORY_DELIMITER "\\"

const char* get_level_string( log_level logging_level_ )
{
    static const char* level_strings[] = {"FATAL ", "ERROR ", "WARN ", "INFO ", "DEBUG ", "TRACE "};
    return level_strings[ static_cast<int>( logging_level_ ) ];
}

namespace logger_queue
{
    bool bRunning = false;

    typedef
    struct log_entry
    {
        log_level logging_level;
        std::string message;
        time_t time_stamp;
    }log_entry;

    std::thread queue_worker_id;
    std::mutex log_mutex;
    std::queue<log_entry> log_queue;
    const std::chrono::seconds QUEUE_DELAY = std::chrono::seconds(5);

    void write_log_queue_worker()
    {
        CONSOLE_INFO( " Log Queue Worker Started ");
        std::stringstream file_name;

        {
            auto now = std::chrono::system_clock::now();
            auto date_stamp = std::chrono::system_clock::to_time_t( now );

            namespace fs = std::filesystem;

            if( !fs::is_directory("logs") || !fs::exists("logs") )
            {
                fs::create_directory("logs");
            }

            file_name << "." << DIRECTORY_DELIMITER << "logs" << DIRECTORY_DELIMITER << std::put_time( std::localtime( &date_stamp ), "%Y-%m-%d_%OH-%M-%S" ) << "_viverrinus.log";
            CONSOLE_INFO( " Log Filename %s ", file_name.str().c_str() );
        }

        std::ofstream log_out_file;
        log_out_file.open( file_name.str(), std::ofstream::out | std::ofstream::app );

        if( !log_out_file.is_open() )
        {
            CONSOLE_INFO( " File Failed to open! ");
        }

        while ( bRunning || !log_queue.empty() )
        {
            std::stringstream log_message;
            log_entry log;

            if( !log_queue.empty() )
            {

                {
                    std::lock_guard<std::mutex> lock( log_mutex );
                    log = log_queue.front();
                    log_queue.pop();
                }

                log_message << std::put_time( std::localtime( &log.time_stamp ), "%Y-%m-%d %X" ) << " ";
                log_message << get_level_string( log.logging_level ) << " ";
                log_message << log.message << std::endl;
                log_out_file << log_message.str();
            }

            std::this_thread::sleep_for( QUEUE_DELAY );
        }

        log_out_file.close();
        CONSOLE_INFO( " Log Queue Worker Exited ");
    }
}

uint8_t initialize_logging(  )
{
    logger_queue::bRunning = true;
    logger_queue::queue_worker_id = std::thread( logger_queue::write_log_queue_worker );

    return 0;
}

void shutdown_logging()
{
    logger_queue::bRunning = false;
    logger_queue::queue_worker_id.join();
}

void log_output(log_level level_, const char * msg_, ...)
{
    char log_msg[ MAX_MESSAGE_SIZE ] = {};
    va_list args_ptr;

    va_start( args_ptr, msg_ );
    vsnprintf( log_msg, MAX_MESSAGE_SIZE, msg_, args_ptr );
    va_end( args_ptr );

    time_t now = time(nullptr);
    logger_queue::log_entry new_log{ level_, std::string( log_msg ), now};

    {
        std::lock_guard<std::mutex> lock( logger_queue::log_mutex );
        logger_queue::log_queue.push(new_log);
    }
}



void console_output( log_level level_, const char * msg_, ... )
{
     static std::mutex stdout_mutex;
    char log_msg[ MAX_MESSAGE_SIZE ] = {};
    va_list args_ptr;

    va_start( args_ptr, msg_ );
    vsnprintf( log_msg, MAX_MESSAGE_SIZE, msg_, args_ptr );
    va_end( args_ptr );

    time_t now = time(nullptr);

    {
        std::lock_guard<std::mutex> lock( stdout_mutex );
        std::cout << std::put_time( std::localtime( &now ), "%Y-%m-%d %X" ) << " " << get_level_string( level_ ) << log_msg << std::endl;
    }
}

void report_assertion_failure(const char *expression_, 
                              const char * msg_, 
                              const char * file_, 
                              uint32_t line_)
{
    log_output( LOG_LEVEL_FATAL, "Assertion Failure: %s , message : '%s' , in file: %s, line: %d", expression_, msg_, file_, line_);
}