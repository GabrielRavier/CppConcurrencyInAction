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
    std::queue<T> m_dataQueue;
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

    void push(T newVal)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_dataQueue.push(newVal);
        m_dataCond.notify_one();
    }

    bool tryPop(T& val);    // Store retrieved value in the referenced variable, retval for status
    std::shared_ptr<T> tryPop(); // Pointer being nullptr for status

    void waitAndPop(T& value)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_dataCond.wait(lock, [this] {return !m_dataQueue.empty();});
        value = m_dataQueue.front();
        m_dataQueue.pop();
    }
    std::shared_ptr<T> waitAndPop();

    bool empty() const;
};

}