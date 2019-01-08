#pragma once
#include <memory>
#include <mutex>
#include <queue>
#include <condition_variable>

namespace ravier
{

template<typename T> class ThreadSafeQueue
{
private:
    mutable std::mutex m_mutex;
    std::queue<std::shared_ptr<T>> m_dataQueue;
    std::condition_variable m_dataCond;

public:
    ThreadSafeQueue()
    {

    }

    ThreadSafeQueue(const ThreadSafeQueue& other)
    {
        std::lock_guard<std::mutex> lock(other.m_mutex);
        m_dataQueue = other.m_dataQueue;
    }

    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete; // Disallow assignment for simplicity

    void waitAndPop(T& value)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_dataCond.wait(lock, [this] {return !m_dataQueue.empty();});
        value = std::move(*m_dataQueue.front());
        m_dataQueue.pop();
    }

    // Store retrieved value in the referenced variable, retval to check for failure
    bool tryPop(T& val)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_dataQueue.empty())
            return false;   // No value could be retrieved
        val = std::move(*m_dataQueue.front());
        m_dataQueue.pop();
        return true;
    }

    std::shared_ptr<T> waitAndPop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_dataCond.wait(lock, [this]{return !m_dataQueue.empty();});
        auto result = m_dataQueue.front();
        m_dataQueue.pop();
        return result;
    }

    // Return nullptr if fail
    std::shared_ptr<T> tryPop()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_dataQueue.empty())
            return std::shared_ptr<T>();    // No value could be retrieved
        auto result = m_dataQueue.front();
        m_dataQueue.pop();
        return result;
    }

    void push(T newVal)
    {
        // Allocation can be done before getting the lock
        auto ptr = std::make_shared<T>(std::move(newVal)));
        
        std::lock_guard<std::mutex> lock(m_mutex);
        m_dataQueue.push(ptr);
        m_dataCond.notify_one();
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_dataQueue.empty();
    }
};

}