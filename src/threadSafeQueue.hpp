#pragma once
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

namespace ravier
{

template <typename T> class threadSafeQueue
{
private:
    mutable std::mutex m_mutex; // The mutex must be mutable
    std::queue<T> m_queue;
    std::condition_variable m_queueCond;

public:
    threadSafeQueue()
    {

    }

    threadSafeQueue(threadSafeQueue const& other)
    {
        std::lock_guard<std::mutex> lock(other.m_mutex);
        m_queue = other.m_queue;
    }

    void push(T newValue)
    {
        std::lock_guard<std::mutex> lock (m_mutex);
        m_queue.push(newValue);
        m_queueCond.notify_one();
    }

    void waitAndPop(T& value)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queueCond.wait(lock, [this]{return !m_queue.empty();});
        value = m_queue.front();
        m_queue.pop();
    }

    std::shared_ptr<T> waitAndPop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queueCond.wait(lock, [this]{return !m_queue.empty();});

        auto result(std::make_shared<T>(m_queue.front()));
        m_queue.pop();
        return result;
    }

    bool tryPop(T& value)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (m_queue.empty())
            return false;

        value = m_queue.front();
        m_queue.pop();
        return true;
    }

    std::shared_ptr<T> tryPop()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queue.empty())
            return std::shared_ptr<T>();
        auto result(std::make_shared<T>(m_queue.front()));
        m_queue.pop();
        return result;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }
};

}
