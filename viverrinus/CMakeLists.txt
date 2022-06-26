cmake_minimum_required(VERSION 3.20)

# set the project name
project(viverrinus VERSION 1.0)

set( INSTALL_INCLUDES src/core/logger
                      src/core/asserts
                      src/platform/platform.h )

#source files
set( CORE_SOURCE_FILES src/core/logger.cpp
                       src/core/application.cpp )

set( PLATFORM_SOURCE_FILES src/platform/platform_win.cpp )

set( RESOURCE_SOURCE_FILES src/resources/loaders/fbx_loader.cpp )

set( CMAKE_CXX_FLAGS " -pthread -g -m64 -std=c++17" )
set( CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++" )

# add the executable
add_library( ${CMAKE_PROJECT_NAME} SHARED ${CORE_SOURCE_FILES}
                                          ${RESOURCE_SOURCE_FILES} 
                                          ${PLATFORM_SOURCE_FILES} )

# 
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(tool_dest "bin")
set(include_dest "include/${CMAKE_PROJECT_NAME}-${MY_LIBRARY_VERSION}")
set(main_lib_dest "lib/${CMAKE_PROJECT_NAME}-${MY_LIBRARY_VERSION}")
