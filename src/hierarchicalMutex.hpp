#pragma once
#include <thread>
#include <mutex>

namespace ravier
{

class HierarchicalMutex
{
private:
    std::mutex m_internalMutex;
    unsigned long const m_hierarchyValue;
    unsigned long m_previousHierarchyValue = 0; // Protected by the lock on the internal mutex when used
    // Hierarchy value for the current thread
    static thread_local unsigned long m_thisThreadHierarchyValue;

    void checkForHierarchyViolation()
    {
        if (m_thisThreadHierarchyValue <= m_hierarchyValue)
            throw std::logic_error("Mutex hierarchy was violated");
    }

    void updateHierarchyValue()
    {
        m_previousHierarchyValue = m_thisThreadHierarchyValue;
        m_thisThreadHierarchyValue = m_hierarchyValue;
    }

public:
    explicit HierarchicalMutex(unsigned long val) : m_hierarchyValue(val)
    {

    }

    void lock()
    {
        this->checkForHierarchyViolation();
        m_internalMutex.lock(); // Delegate to the internal mutex for the actual lock
        this->updateHierarchyValue(); // Update hierarchy value
    }

    void unlock()
    {
        m_thisThreadHierarchyValue = m_previousHierarchyValue; // Restore old hierarchy value
        m_internalMutex.unlock();
    }

    bool try_lock()
    {
        this->checkForHierarchyViolation();
        if (!m_internalMutex.try_lock()) // Check if owning the lock
            return false; // We don't so don't update the hierarchy value
        this->updateHierarchyValue();
        return true;
    }
};

// Initialize m_thisThreadHierarchyValue to the maximum value to allow any mutex to be locked
thread_local unsigned long HierarchicalMutex::m_thisThreadHierarchyValue(ULONG_MAX);

}
