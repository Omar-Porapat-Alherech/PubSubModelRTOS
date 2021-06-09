cmake_minimum_required(VERSION 3.15)
project(Project4 C)

set(CMAKE_C_STANDARD 99)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -lsocket")
#include_directories("/lib/modules/$(shell uname -r)/build/include")
add_executable(Project4 usersspace.c )