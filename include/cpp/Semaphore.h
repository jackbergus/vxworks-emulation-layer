
#ifndef GENERALFRAMEWORK_SEMAPHORE_H
#define GENERALFRAMEWORK_SEMAPHORE_H
#include <cstdint>

class Semaphore {
    void* self{nullptr};
public:
    Semaphore(int options, uint64_t initialValue = 0);
    Semaphore(Semaphore&& x) : self{x.self} {
        x.self = nullptr;
    }
    Semaphore& operator=(Semaphore&& x) {
        self = x.self;
        x.self = nullptr;
        return *this;
    }

    Semaphore(const Semaphore&) = delete;
    Semaphore& operator=(const Semaphore&) = delete;

    bool V();
    bool P(int64_t timeout);

    ~Semaphore();
};


#endif //GENERALFRAMEWORK_SEMAPHORE_H