#include "timer.h"

    bool Timer::isRunning() const 
    {
        return startTime != std::chrono::steady_clock::time_point{};
    }

    void Timer::start() 
    {
        if(!isRunning()) 
        {
            startTime = std::chrono::steady_clock::now();
        }
    }

    void Timer::stop() 
    {
        if(isRunning()) 
        {
            elapsedTime += std::chrono::steady_clock::now() - startTime;
            startTime = {};
        }
    }

    void Timer::reset() 
    {
        startTime = {};
        elapsedTime = {};
    }

    // Return the elapsed time in seconds
    uint64_t Timer::getElapsedSeconds() 
    {
        return std::chrono::duration_cast<std::chrono::duration<uint64_t>>(getElapsed()).count();
    }

    // Return the elapsed time in minutes
    uint64_t Timer::getElapsedMinutes() 
    {
        return std::chrono::duration_cast<std::chrono::duration<uint64_t>>(getElapsed()).count() / 60;
    }

    std::chrono::steady_clock::duration Timer::getElapsed() 
    {
        auto result = elapsedTime;

        if(isRunning()) 
        {
            result += std::chrono::steady_clock::now() - startTime;
        }

        return result;
    }