#pragma once

#include <time.h>

#define NANOSECONDS_PER_SECOND 1000000000

/**
 * Converts an amount of emulated VxWorks ticks to a timespec
 */
struct timespec _ticksToTimespec(int ticks);

/**
 * Returns a timespec that contains the absolute time of the provided ticks
 * relative to now.
 */
struct timespec _ticksToAbsoluteTimespec(int ticks);

/**
 * Converts a timespec to an amount of emulated VxWorks ticks based on the
 * emulated sysClkRate
 */
unsigned long _timespecToTicks(struct timespec spec);

/**
 * Helper function to subtract two timespecs
 */
struct timespec _subtractTimespecs(struct timespec before, struct timespec after);

#if __has_include(<pthread_time.h>)
#include <pthread_time.h>
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined(WIN64)
#include <windows.h>
#define USE_MANUAL_NANOSLEEP 1
static inline BOOLEAN nanosleep(LONGLONG ns) {
    HANDLE timer;
    LARGE_INTEGER li;
    if (!(timer = CreateWaitableTimer(NULL, TRUE, NULL)))
        return FALSE;
    li.QuadPart = -ns;
    if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE)) {
        CloseHandle(timer);
        return FALSE;
    }
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
    return TRUE;
}

#endif
