#include <fstream>
#include <iostream>

#define logFile "log.txt"

#pragma once

// —————————————————— Log to File ——————————————————
class logToFile
{
public:
    logToFile();

    ~logToFile();

    template <typename T>
    logToFile &operator<<(const T &value);

private:
    std::ofstream file;
};