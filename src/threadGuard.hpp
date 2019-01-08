#pragma once
#include <thread>

namespace ravier
{

class ThreadGuard
{
private:
    std::thread& m_thread;

public:
    explicit ThreadGuard(std::thread& t) : m_thread(t)
    {

    }

    ~ThreadGuard()
    {
        if (m_thread.joinable()) // Join if joinable
            m_thread.join();
    }

    ThreadGuard(ThreadGuard const&) = delete; // Disable copy constructor and copy-assignment operators
    ThreadGuard& operator=(ThreadGuard const&) = delete;
};

}
