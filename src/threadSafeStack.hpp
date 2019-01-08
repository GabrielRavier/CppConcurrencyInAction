#pragma once
#include <exception>
#include <memory>
#include <mutex>
#include <stack>

namespace ravier
{

struct EmptyStack : std::exception
{
	const char *what() const noexcept
	{
		"Attempted use of empty stack";
	};
};

template <typename T> class ThreadSafeStack
{
private:
	std::stack<T> m_actualStack;
	mutable std::mutex m_mutex;

public:
	ThreadSafeStack()
	{

	}
	ThreadSafeStack(const ThreadSafeStack& other)
	{
		std::lock_guard<std::mutex> lock(other.m_mutex);
		// Copy performed in constructor body to ensure that the mutex is help across the copy
		m_actualStack = other.m_actualStack;
	}

	void push(T newValue)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_actualStack.push(std::move(newValue));	// May throw but std::stack guarantees it'll be safe 
	}

	std::shared_ptr<T> pop()
	{
	    std::lock_guard<std::mutex> lock(m_mutex);
	    if (m_actualStack.empty())	// Check for empty before trying to pop value
            throw EmptyStack();	// Nothing changed so no problems
		// Allocate return value before modifying stack
		// May throw but std guarantees no memory leaks
        auto const result(std::make_shared<T>(std::move(m_actualStack.top())));
        m_actualStack.pop(); // Cannot throw
        return result;
	}

	void pop(T& value)
	{
	    std::lock_guard<std::mutex> lock(m_mutex);
	    if (m_actualStack.empty())
            throw EmptyStack();
        value = std::move(m_actualStack.top()); // Can throw but np
        m_actualStack.pop(); // Cannot throw
	}

	bool empty() const
	{
	    std::lock_guard<std::mutex> lock(m_mutex);
	    return m_actualStack.empty();
	}
};

}
