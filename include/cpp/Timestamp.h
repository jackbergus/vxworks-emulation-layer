//
// Created by Giacomo Bergami, PhD on 10/04/2026.
//

#ifndef GENERALFRAMEWORK_TIMESTAMP_H
#define GENERALFRAMEWORK_TIMESTAMP_H

#include <chrono>
#include <ratio>

template<typename time_granularity = std::milli>
uint32_t getTimestamp() {
    return static_cast<uint32_t>(std::chrono::duration<double, time_granularity>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
}

#endif //GENERALFRAMEWORK_TIMESTAMP_H