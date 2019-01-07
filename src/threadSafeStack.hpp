#pragma once
#include <exception>
#include <memory>
#include <mutex>
#include <stack>

namespace ravier
{

struct emptyStack : std::exception
{
	const char *what() const noexcept;
};

template <typename T> class threadSafeStack
{
private:
	std::stack<T> m_actualStack;
	mutable std::mutex m_mutex;

public:
	threadSafeStack()
	{

	}
	threadSafeStack(const threadSafeStack& other)
	{
		std::lock_guard<std::mutex> lock(other.m_mutex);
		m_actualStack = other.m_actualStack;
	}

	void push(T newValue)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_actualStack.push(newValue);
	}

	std::shared_ptr<T> pop()
	{
	    std::lock_guard<std::mutex> lock(m_mutex);
	    if (m_actualStack.empty())
            throw emptyStack();
        auto const result(std::make_shared<T>(m_actualStack.top()));
        m_actualStack.pop();
        return result;
	}

	void pop(T& value)
	{
	    std::lock_guard<std::mutex> lock(m_mutex);
	    if (m_actualStack.empty())
            throw emptyStack();
        value = m_actualStack.top();
        m_actualStack.pop();
	}

	bool empty() const
	{
	    std::lock_guard<std::mutex> lock(m_mutex);
	    return m_actualStack.empty();
	}
};

}
