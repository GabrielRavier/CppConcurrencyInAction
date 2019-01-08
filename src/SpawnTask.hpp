#pragma once
#include <future>
#include <type_traits>

namespace ravier
{

template<typename F, typename A> std::future<typeof(std::result_of<F(A&&)>::type)> spawnTask(F&& f, A&& a)
{
    typedef typeof(std::result_of<F(A&&)>::type) ResultType;

    std::packaged_task<ResultType(A&&)> task(std::move(f));
    std::future<ResultType> result(task.get_future());
    std::thread t(std::move(task), std::move(a));
    t.detach();
    return result;
}

}