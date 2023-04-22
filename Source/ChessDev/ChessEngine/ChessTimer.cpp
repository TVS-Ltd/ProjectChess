// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessTimer.h"

bool ChessTimer::isRunning() const 
{
    return startTime != std::chrono::steady_clock::time_point{};
}

void ChessTimer::start() 
{
    if(!isRunning()) 
    {
        startTime = std::chrono::steady_clock::now();
    }
}

void ChessTimer::stop() 
{
    if(isRunning()) 
    {
        elapsedTime += std::chrono::steady_clock::now() - startTime;
        startTime = {};
    }
}

void ChessTimer::reset() 
{
    startTime = {};
    elapsedTime = {};
}

// Return the elapsed time in seconds
uint64_t ChessTimer::getElapsedSeconds() 
{
    return std::chrono::duration_cast<std::chrono::duration<uint64_t>>(getElapsed()).count();
}

// Return the elapsed time in minutes
uint64_t ChessTimer::getElapsedMinutes() 
{
    return std::chrono::duration_cast<std::chrono::duration<uint64_t>>(getElapsed()).count() / 60;
}

std::chrono::steady_clock::duration ChessTimer::getElapsed() 
{
    auto result = elapsedTime;

    if(isRunning()) 
    {
        result += std::chrono::steady_clock::now() - startTime;
    }

    return result;
}

ChessTimer::ChessTimer()
{
}

ChessTimer::~ChessTimer()
{
}
