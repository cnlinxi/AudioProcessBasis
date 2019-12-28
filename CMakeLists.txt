cmake_minimum_required(VERSION 2.8)
project(AudioProcessBasis)

aux_source_directory(. DIR_SRCS)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR})

set(PROJECT_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/dr_wav.h
        ${PROJECT_SOURCE_DIR}/timing.h
        ${PROJECT_SOURCE_DIR}/main.c
        )

add_executable(audio_process ${PROJECT_SOURCE_FILES})