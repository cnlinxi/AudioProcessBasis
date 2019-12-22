cmake_minimum_required(VERSION 2.8)
project(AudioProcess LANGUAGES C)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
SET(CMAKE_BUILD_TYPE "Debug")

#set(CMAKE_C_STANDARD 99)
include_directories(include)
add_executable(AudioProcess main.c)

target_link_libraries(AudioProcess -lm)