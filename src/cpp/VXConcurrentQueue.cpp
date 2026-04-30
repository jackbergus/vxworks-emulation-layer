//
// Created by gyankos on 06/03/26.
//

#include <cpp/VXConcurrentQueue.h>
extern "C" {
    #include <msgQLib.h>
}
VXConcurrentQueue::VXConcurrentQueue(int maxMsgs, int maxMsgLength, int options) {
    self = msgQCreate(maxMsgs, maxMsgLength, options);
}

bool VXConcurrentQueue::push(void *ptr, uint64_t len, int timeout, int priority) {
        return (msgQSend(self, (char*) ptr, len, timeout, priority) == 0);
}


#include <algorithm>

int64_t VXConcurrentQueue::pop(void* buffer, uint64_t bufferSize, int timeout) {
    bufferSize = std::min((uint64_t)((vxworksMsgQ_t* )self)->maxMsgLength, bufferSize);
    auto idx = msgQReceive(self, (char*)buffer, bufferSize, timeout);
    if (idx < 0) {
        return -1;
    }
    return bufferSize;
}

uint64_t VXConcurrentQueue::size() {
    return msgQNumMsgs(self);
}

VXConcurrentQueue::~VXConcurrentQueue() {
    msgQDelete(self);
}
