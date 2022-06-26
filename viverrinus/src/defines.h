#pragma once

#include "stdint.h"

// Platform detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #define __PLATFORM_WINDOWS__ 1
#ifndef _WIN64
    #error "64-bit is required on Windows!"
#endif
#elif defined(__linux__) || defined(__gnu_linux__)
// Linux OS
    #define __PLATFORM_LINUX__
    #if defined(__ANDROID__)
        #define __PLATFORM_ANDROID__ 1
    #endif
    #elif defined(__unix__)
// Catch anything not caught by the above.
        #define KPLATFORM_UNIX 1
    #elif defined(_POSIX_VERSION)
// Posix
    #define KPLATFORM_POSIX 1
#else
    #error "Unknown platform!"
#endif

#ifdef _MSC_VER
#define API __declspec(dllexport)
#else
#define API __attribute__((visibility("default")))
#endif