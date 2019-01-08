#pragma once
#include <numeric>
#include <iterator>
#include <thread>
#include <cstddef>
#include <algorithm>
#include <vector>
#include <functional>

namespace ravier
{

template<typename Iterator, typename T> struct accumulateBlock
{
    void operator()(Iterator first, Iterator last, T& result)
    {
        result = std::accumulate(first, last, result);
    }
};

template <typename Iterator, typename T> T parallelAccumulate(Iterator first, Iterator last, T init)
{
    const auto length = std::distance(first, last);

    if (!length)    // If the input range is empty, return the initial value
        return init;

    constexpr size_t minPerThread = 25;
    const auto maxThreads = (length + minPerThread - 1) / minPerThread;

    const auto hardwareThreads = std::thread::hardware_concurrency();

    // Number of threads to run is the minimum of the calculated maximum and the number of hardware threads
    const auto numThreads = std::min(hardwareThreads != 0 ? hardwareThreads : 2, maxThreads); 

    const auto blockSize = length / numThreads;

    std::vector<T> results(numThreads); 
    // One fewer threads than numThreads since we already have one (the one that's executing this code)
    std::vector<std::thread> threads(numThreads - 1);

    Iterator blockStart = first;
    for (size_t i = 0; i < (numThreads - 1); ++i)
    {
        Iterator blockEnd = blockStart;
        std::advance(blockEnd, blockSize); // Advance blockEnd to the end of the current block
        // Launch new thread to accumulate block
        threads[i] = std::thread(accumulateBlock<Iterator, T>(), blockStart, blockEnd, std::ref(results[i]));
        blockStart = blockEnd; // Start of next block is the end of the current one
    }

    // Process final block with current thread
    accumulateBlock<Iterator, T>()(blockSize, last, results[numThreads - 1]);

    // Wait for all the threads
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join)); 

    return std::accumulate(results.begin(), results.end(), init); // Add up the results
}

}