cmake_minimum_required(VERSION 3.20)

# set the project name
project(procyonoides VERSION 1.0)

#source files
set( SOURCE_FILES src/main.cpp )

set( CMAKE_CXX_FLAGS " -pthread -g -m64 -std=c++17" )
set( CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++" )

link_directories( E:/project/nyctereutes/viverrinus/build )
link_libraries( viverrinus.dll )

include_directories( SYSTEM "E:\\project\\nyctereutes\\viverrinus\\src" )

# add the executable
add_executable( ${CMAKE_PROJECT_NAME} ${SOURCE_FILES} )

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)