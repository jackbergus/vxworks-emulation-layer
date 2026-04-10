//
// Created by gyankos on 06/03/26.
//

#include "cpp/Semaphore.h"

extern "C" {
    #include "semLib.h"
}


Semaphore::Semaphore(int options, uint64_t initialValue) {
    self = semCCreate(options, initialValue);
}

bool Semaphore::V() {
    if (self) {
        return semGive(self) == 0;
    } else
        return false;

}

bool Semaphore::P(int64_t timeout) {
    if (self) {
        return semTake(self, timeout) == 0;
    } else {
        return false;
    }
}

Semaphore::~Semaphore() {
    if (self) {
        semDelete(self);
        self = nullptr;
    }

}
