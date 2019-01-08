#pragma once
#include <thread>

namespace ravier
{

class ScopedThread
{
private:
	std::thread m_thread;

public:
	explicit ScopedThread(std::thread t) : m_thread(std::move(t))
	{
		if (!m_thread.joinable()) // Throw exception if thread is not joinable
			throw std::logic_error("Thread was not joinable (no thread)");
	}

	~ScopedThread()
	{
		m_thread.join();
	}

	ScopedThread(ScopedThread const&) = delete;
	ScopedThread& operator=(ScopedThread const&) = delete;
};

}
