
#ifndef GENERALFRAMEWORK_SEMAPHORE_H
#define GENERALFRAMEWORK_SEMAPHORE_H
#include <cstdint>

class Semaphore {
    void* self;
public:
    Semaphore(int options, uint64_t initialValue = 0);

    bool V();
    bool P(int64_t timeout);

    ~Semaphore();
};


#endif //GENERALFRAMEWORK_SEMAPHORE_H