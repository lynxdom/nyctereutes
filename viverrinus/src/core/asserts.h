#pragma once 

#include "../defines.h"

void report_assertion_failure(const char *expression_, 
                                const char * msg_, 
                                const char * file_, 
                                uint32_t line_);

#define V_ASSERTIONS_ENABLED

#ifdef V_ASSERTIONS_ENABLED
    #if _MSC_VER
        #include <intrin.h>
        #define debugBreak() __debugbreak()
    #else
        #define debugBreak() __builtin_trap()
    #endif

    #define V_ASSERT(expr)\
    {\
        if(expr)\
        {\
            NULL;\
        }\
        else\
        {\
            report_assertion_failure( #expr, "", __FILE__, __LINE__);\
            debugBreak();\
        }\
    }\

    #define V_ASSERT_MSG(expr, msg)\
    {\
        if(expr)\
        {\
            NULL;\
        }\
        else\
        {\
            report_assertion_failure( #expr, #msg, __FILE__, __LINE__);\
            debugBreak();\
        }\
    }\

    #ifdef _DEBUG
        #define V_ASSERT_DEBUG(expr)\
        {\
            if(expr)\
            {\
                NULL;\
            }\
            else\
            {\
                report_assertion_failure( #expr, "", __FILE__, __LINE__);\
                debugBreak();\
            }\
        }\

        #define V_ASSERT_MSG_DEBUG(expr, msg)\
        {\
            if(expr)\
            {\
                NULL;\
            }\
            else\
            {\
                report_assertion_failure( #expr, #msg, __FILE__, __LINE__);\
                debugBreak();\
            }\
        }\

    #else
        #define V_ASSERT_MSG_DEBUG(expr, msg)
        #define KASSERT_DEBUG(expr)
    #endif

#else
    #define V_ASSERT_MSG(expr, msg)
    #define V_ASSERT_MSG_DEBUG(expr, msg)
    #define V_ASSERT_DEBUG(expr)
    #define V_ASSERT(expr)
#endif