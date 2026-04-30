//
// Created by gyankos on 06/03/26.
//

extern "C"{
#include <wdLib.h>
    }
#include "cpp/WatchDog.h"

WatchDog::WatchDog(int ticks, FUNCPTR pRoutine, size_t parameter):  delay(ticks), routine(pRoutine), parameter(parameter) {
    self = wdCreate();
}

void WatchDog::start() {
    wdStart(self, delay, routine, parameter);
}

WatchDog::~WatchDog() {
    wdDelete(self);
}


void WatchDog::cancel() {
    wdCancel(self);
}
