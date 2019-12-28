//
// Created by Mengnan on 2019/12/22.
//

#include <stdint.h>

#if defined(__APPLE__)

#include <mach/mach_time.h>

#elif defined(_WIN32)
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#else
#include <time.h>

#ifndef AUDIOPROCESS_TIMING_H
#define AUDIOPROCESS_TIMING_H
#endif
#endif //AUDIOPROCESS_TIMING_H

static
uint64_t nanotimer() {
    static int ever = 0;
#if defined(__APPLE__)
    static mach_timebase_info_data_t frequency;
    if (!ever) {
        if (mach_timebase_info(&frequency) != KERN_SUCCESS) {
            return 0;
        }
        ever = 1;
    }
    return (mach_absolute_time() * frequency.numer / frequency.denom);
#elif defined(_WIN32)
    static LARGE_INTEGER frequency;
    if (!ever) {
        QueryPerformanceFrequency(&frequency);
        ever = 1;
    }
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    return (t.QuadPart * (uint64_t) 1e9) / frequency.QuadPart;
#else
    struct timespec t;
    if(!ever){
        if(clock_gettime(CLOCK_MONOTONIC,&t)!=0){
            return 0;
        }
        ever=1;
    }
    clock_gettime(CLOCK_MONOTONIC,&t);
    return (t.tv_sec*(uint64_t)1e9)+t.tv_nsec;
#endif
}

static double now() {
    static uint64_t epoch = 0;
    if (!epoch) {
        epoch = nanotimer();
    }
    return (nanotimer() - epoch) / 1e9;
}

double calcElapsed(double start, double end) {
    double took = -start;
    return took + end;
}
