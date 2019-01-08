#include <mutex>
#include <condition_variable>
#include <chrono>

namespace ravier
{

inline bool waitForCondVar(std::condition_variable& cv, const bool& done, std::mutex& m)
{
    const auto timeout = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
    std::unique_lock<std::mutex> lock(m);
    while (!done)
        if (cv.wait_until(lock, timeout) == std::cv_status::timeout)
            break;
    return done;
}

}