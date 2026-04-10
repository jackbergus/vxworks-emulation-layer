#include "sysLib.h"
#include "helpers.h"
#include <stddef.h>
#include <time.h>
#include "tickLib.h"
#include <stdlib.h>

#define DEFAULT_SYS_CLK_RATE 500

int g_sysClkRate = DEFAULT_SYS_CLK_RATE;
extern struct timespec* g_lastUpdateTime;
extern unsigned long g_baseKernelTicks;

/**************************** Library Functions ******************************/

int sysClkRateGet(void) {
    return g_sysClkRate;
}

STATUS sysClkRateSet(int ticksPerSecond) {
    // This is great and all, but we need to recalculate the emulated
    // tickLib g__kernelTicks, because otherwise it will be out of sync
    // with being updated by the new clock rate

    struct timespec now;
#ifdef CLOCK_BOOTTIME
    clock_gettime(CLOCK_BOOTTIME, &now);
#else
    clock_gettime(CLOCK_REALTIME, &now);
#endif

    if (g_lastUpdateTime == NULL) {
        g_lastUpdateTime = (struct timespec*) malloc(sizeof(struct timespec));

#ifdef CLOCK_BOOTTIME
        clock_gettime(CLOCK_BOOTTIME, g_lastUpdateTime);
#else
        clock_gettime(CLOCK_REALTIME, g_lastUpdateTime);
#endif

    }

    struct timespec elapsed = _subtractTimespecs(*g_lastUpdateTime, now);

    g_baseKernelTicks += _timespecToTicks(elapsed);

    *g_lastUpdateTime = now;

    g_sysClkRate = ticksPerSecond;

    return 0;
}
