//
// Created by gyankos on 06/03/26.
//

#include "cpp/Semaphore.h"

#include "semLib.h"

Semaphore::Semaphore(int options, uint64_t initialValue) {
    self = semCCreate(options, initialValue);
}

bool Semaphore::V() {
    return semGive(self) == 0;
}

bool Semaphore::P(int64_t timeout) {
    return semTake(self, timeout) == 0;
}

Semaphore::~Semaphore() {
    semDelete(self);
}
