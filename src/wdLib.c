#include "wdLib.h"
#include "helpers.h"
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>

// TODO: in windows, use waitable timers

/************************** Helper structs ***********************************/

#if defined(_WIN64) || defined(_WIN32)
#else
typedef struct {
    timer_t* inner;
} vxworksWdTimer_t;
#endif

typedef struct {
    void* timerArg;
    FUNCPTR callback;
} vxworksWdTimerPreArgs_t;

/************************** Helper functions ***********************************/

void _timerPreFunction(union sigval);

/************************ Library functions **********************************/

WDOG_ID wdCreate(void) {
#if defined(_WIN64) || defined(_WIN32)
    return CreateWaitableTimer(NULL, TRUE, NULL);
#else
    // POSIX timers need a routine that should be called to be specified
    // when the timer is created instead of timer start time.
    //
    // Due to this, we will use two levels of indirection to the underlying
    // timer_t, that way the pointer to it can be NULL if we haven't previously
    // called wdStart on it yet.

    vxworksWdTimer_t* timer = (vxworksWdTimer_t*) malloc(sizeof(vxworksWdTimer_t));

    timer->inner = NULL;

    return (void*) timer;
#endif
}

STATUS wdDelete(WDOG_ID wdId) {
#if defined(_WIN64) || defined(_WIN32)
    return CloseHandle(wdId) ? VX_OK : VX_ERROR;
#else
    vxworksWdTimer_t* timer = (vxworksWdTimer_t*) wdId;
    int status = 0;

    if (timer == NULL) {
        return -1;
    }

    if (timer->inner != NULL) {
        status = timer_delete(*(timer->inner));
    }

    free((void*) timer->inner);
    free((void*) timer);

    return status;
#endif
}

STATUS wdStart(WDOG_ID wdId, int delay, FUNCPTR pRoutine, size_t parameter) {
#if defined(_WIN64) || defined(_WIN32)
    if (wdId == NULL) {
        return VX_ERROR;
    }

    LARGE_INTEGER   liDueTime;
    struct timespec spec = _ticksToTimespec(delay);
    uint64_t val = spec.tv_sec * NANOSECONDS_PER_SECOND/100 + spec.tv_nsec/100;
    liDueTime.LowPart  = (DWORD) ( val & 0xFFFFFFFF );
    liDueTime.HighPart = (LONG)  ( val >> 32 );

    return SetWaitableTimer(wdId, &liDueTime, 0, (PTIMERAPCROUTINE)pRoutine, (LPVOID)parameter, false) ? VX_OK : VX_ERROR;
#else
    vxworksWdTimer_t* timer = (vxworksWdTimer_t*) wdId;

    if (timer == NULL) {
        return -1;
    }

    // Here we actually create the POSIX timer
    //
    // Unfortunately, we will have to delete and re-create a timer every time
    // this function is called. This is because as said in the wdCreate()
    // function, POSIX timers register the callback routine at timer creation,
    // not timer start, like VxWorks.

    if (timer->inner != NULL) {
        // If deletion of the previous timer fails, well we should probably
        // keep going. If this emulation layer needed to run a real real-time
        // system, then we should have this whole function fail here.
        timer_delete(*(timer->inner));
    } else {
        timer->inner = (timer_t*) malloc(sizeof(timer_t));
    }

    // NOTE: The sigevent struct is meant to only have certain parts of it set.
    // This fact is brought up in the documentation, but in the example there 
    // is no use of memset() to clear it before using it. When reusing the same
    // memory for multiple timer_create calls, it seems like it is actually
    // required, or we end up segfaulting.
    
    vxworksWdTimerPreArgs_t* preArgs = malloc(sizeof(vxworksWdTimerPreArgs_t));
    preArgs->timerArg = (void*) parameter;
    preArgs->callback = pRoutine;

    struct sigevent ev;

    memset(&ev, 0, sizeof(struct sigevent));

    ev.sigev_notify = SIGEV_THREAD;
    ev.sigev_value.sival_ptr = preArgs;
    ev.sigev_notify_function = _timerPreFunction;

    int status = timer_create(CLOCK_REALTIME, &ev, timer->inner);

    if (status != 0) {
        return -1;
    }

    // Now to start the timer

    // A zero interval means that the timer will only go off once -- the
    // behavior of VxWorks watchdog timers
    struct timespec it_interval;
    it_interval.tv_sec = 0;
    it_interval.tv_nsec = 0;

    // This is a relative timeout
    struct timespec it_value = _ticksToTimespec(delay);

    struct itimerspec timer_value;
    timer_value.it_interval = it_interval;
    timer_value.it_value = it_value;

    return timer_settime(*(timer->inner), 0, &timer_value, NULL);
#endif
}

STATUS wdCancel(WDOG_ID wdId) {
#if defined(_WIN64) || defined(_WIN32)
    return CloseHandle(wdId) ? VX_OK : VX_ERROR;
#else
    // We sort of have to delete the timer here, because if the user wishes
    // to wdStart() the timer again, then they may supply a different pRoutine
    //
    // Deleting cancels the timer

    vxworksWdTimer_t* timer = (vxworksWdTimer_t*) wdId;

    if (timer == NULL) {
        return -1;
    }

    if (timer->inner == NULL) {
        return -1;
    }

    int status = timer_delete(*(timer->inner));

    free((void*) timer->inner);
    timer->inner = NULL;
    return 0;
#endif
}

/************************** Helper functions ***********************************/
#if !(defined(_WIN64) || defined(_WIN32))
void _timerPreFunction(union sigval value) {
    vxworksWdTimerPreArgs_t* preArgs = value.sival_ptr;
    void (*callback)(void*) = (void (*)(void*)) preArgs->callback;
    void* argument = preArgs->timerArg;

    free(preArgs);

    callback(argument);
}
#endif