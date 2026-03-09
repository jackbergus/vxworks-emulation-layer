//
// Created by gyankos on 06/03/26.
//

#ifndef GENERALFRAMEWORK_WATCHDOG_H
#define GENERALFRAMEWORK_WATCHDOG_H

class WatchDog {
    void* self;
    int delay;
    FUNCPTR routine;
    size_t parameter;

public:
    WatchDog(int ticks, FUNCPTR pRoutine, size_t parameter);
    void start();
    void cancel();
    ~WatchDog();
};



#endif //GENERALFRAMEWORK_WATCHDOG_H
