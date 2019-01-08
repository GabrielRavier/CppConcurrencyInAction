#pragma once
#include <atomic>

namespace ravier
{

// This is enough to use with lock_guard lol
class SpinlockMutex
{
private:
    std::atomic_flag flag;

public:
    SpinlockMutex() : flag(ATOMIC_FLAG_INIT)
    {

    }

    void lock()
    {
        while (flag.test_and_set(std::memory_order_acquire))
            ;
    }

    void unlock()
    {
        flag.clear(std::memory_order_release);
    }
};

}