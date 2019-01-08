#pragma once

#include <memory>

namespace ravier
{

template<typename T> class ThreadSafeQueue
{
public:
    ThreadSafeQueue();
    ThreadSafeQueue(const ThreadSafeQueue&);
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete; // Disallow assignment for simplicity
}

}