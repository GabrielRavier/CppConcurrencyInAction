#pragma once
#include <thread>

namespace ravier
{

class threadGuard
{
private:
    std::thread& m_thread;

public:
    explicit threadGuard(std::thread& t) : m_thread(t)
    {

    }

    ~threadGuard()
    {
        if (m_thread.joinable())
            m_thread.join();
    }

    threadGuard(threadGuard const&) = delete;
    threadGuard& operator=(threadGuard const&) = delete;
};

}
