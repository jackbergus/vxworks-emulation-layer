#pragma once

#include "vxworks.h"
#include <stddef.h>

#if defined(_WIN64) || defined(_WIN32)
#include <windows.h>
typedef HANDLE  WDOG_ID;
#else
typedef void*   WDOG_ID;
#endif

/**
 * Create a new VxWorks style watchdog timer
 */
WDOG_ID wdCreate(void);

/**
 * Delete a watchdog timer
 */
STATUS wdDelete(WDOG_ID wdId);

/**
 * Start a watchdog timer
 */
STATUS wdStart(WDOG_ID wdId, int delay, FUNCPTR pRoutine, size_t parameter);

/**
 * Cancel a currently counting watchdog
 */
STATUS wdCancel(WDOG_ID wdId);
