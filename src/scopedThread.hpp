#pragma once
#include <thread>

namespace ravier
{

class scopedThread
{
private:
	std::thread m_thread;

public:
	explicit scopedThread(std::thread t) : m_thread(std::move(t))
	{
		if (!m_thread.joinable())
			throw std::logic_error("Thread was not joinable (no thread)");
	}

	~scopedThread()
	{
		m_thread.join();
	}

	scopedThread(scopedThread const&) = delete;
	scopedThread& operator=(scopedThread const&) = delete;
};

}
