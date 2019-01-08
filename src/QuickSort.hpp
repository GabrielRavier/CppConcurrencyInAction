#pragma once
#include <list>
#include <algorithm>
#include <future>

namespace ravier
{

template<typename T> std::list<T> sequentialQuickSort(const std::list<T>& input)
{
    if (input.empty())
        return input;

    std::list<T> result;
    // Splice first element off the front of the list
    result.splice(result.begin(), input, input.begin());
    const T& pivot = *result.begin();   // Get reference to avoid copying

    // Divide the sequence into less-than more-than halves
    auto dividePoint = std::partition(input.begin(), input.end(), [&](const T& t) {return t < pivot;});

    // Get values until divide point
    std::list<T> lowerPart;
    lowerPart.splice(lowerPart.end(), input, input.begin(), dividePoint);

    // Sort the two lists with recursive calls. We avoid copying with std::move
    auto newLower(sequentialQuickSort(std::move(lowerPart)));
    auto newHigher(sequentialQuickSort(std::move(input)));

    result.splice(result.end(), newHigher); // newHigher goes at the end
    result.splice(result.begin(), newLower);    // newLower goes at the start
    return result;
}

template<typename T> std::list<T> parallelQuickSort(const std::list<T>& input)
{
    if (input.empty())
        return input;

    std::list<T> result;
    // Splice first element off the front of the list
    result.splice(result.begin(), input, input.begin());
    const T& pivot = *result.begin();   // Get reference to avoid copying

    // Divide the sequence into less-than more-than halves
    auto dividePoint = std::partition(input.begin(), input.end(), [&](const T& t) {return t < pivot;});

    // Get values until divide point
    std::list<T> lowerPart;
    lowerPart.splice(lowerPart.end(), input, input.begin(), dividePoint);

    // Sort the lower part on another thread
    std::future<std::list<T>> newLower(std::async(&parallelQuickSort<T>, std::move(lowerPart)));
    auto newHigher(parallelQuickSort(std::move(input)));    // Do the upper part with recursion as before

    result.splice(result.end(), newHigher); // newHigher goes at the end
    result.splice(result.begin(), newLower.get());    // newLower goes at the start
    return result;
}
    
}