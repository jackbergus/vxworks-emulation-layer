#pragma once

#include <stdint.h>

#include "vxworks.h"

int sysClkRateGet(void);
STATUS sysClkRateSet(int ticksPerSecond);

#ifdef _MSC_VER
#include <sysinfoapi.h>
#include <time.h>
#define     CLOCK_REALTIME (0)
static inline int clock_gettime(int obj, struct timespec*spec) {
    int64_t wintime;
    GetSystemTimeAsFileTime((FILETIME*)&wintime);
    wintime -= 116444736000000000i64;
    spec->tv_sec = wintime /  10000000i64;
    spec->tv_nsec = wintime % 10000000i64 * 100;
    return 0;
}

#endif