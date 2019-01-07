#pragma once
#include <thread>
#include <mutex>

namespace ravier
{

class hierarchicalMutex
{
private:
    std::mutex m_internalMutex;
    unsigned long const m_hierarchyValue;
    unsigned long m_previousHierarchyValue;
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
    explicit hierarchicalMutex(unsigned long val) : m_hierarchyValue(val), m_previousHierarchyValue(0)
    {

    }

    void lock()
    {
        checkForHierarchyViolation();
        m_internalMutex.lock();
        updateHierarchyValue();
    }

    void unlock()
    {
        m_thisThreadHierarchyValue = m_previousHierarchyValue;
        m_internalMutex.unlock();
    }

    bool try_lock()
    {
        checkForHierarchyViolation();
        if (!m_internalMutex.try_lock())
            return false;
        updateHierarchyValue();
        return true;
    }
};

thread_local unsigned long hierarchicalMutex::m_thisThreadHierarchyValue(ULONG_MAX);

}
