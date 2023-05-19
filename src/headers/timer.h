#include <chrono>

// —————————————————— Timer ——————————————————
class Timer
{
public:


    std::chrono::steady_clock::time_point startTime = {};
    std::chrono::steady_clock::duration elapsedTime = {};

    bool isRunning() const;

    void start();

    void stop();

    void reset();

    // Return the elapsed time in seconds
    uint64_t getElapsedSeconds();

    // Return the elapsed time in minutes
    uint64_t getElapsedMinutes();

    std::chrono::steady_clock::duration getElapsed();
};