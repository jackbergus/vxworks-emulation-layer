//
// Created by gyankos on 06/03/26.
//

#ifndef GENERALFRAMEWORK_VXCONCURRENTQUEUE_H
#define GENERALFRAMEWORK_VXCONCURRENTQUEUE_H

#include <cstdint>

#include <msgQLib.h>

class VXConcurrentQueue {
    void* self;
public:
    VXConcurrentQueue(int maxMsgs, int maxMsgLength, int options);

    bool push(void* ptr, uint64_t len, int timeout = -1, int priority = MSG_PRI_NORMAL);
    int64_t pop(void* buffer, uint64_t bufferSize, int timeout = -1);
    uint64_t size();

    ~VXConcurrentQueue();
};


#endif //GENERALFRAMEWORK_VXCONCURRENTQUEUE_H