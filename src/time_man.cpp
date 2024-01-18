#include "time_man.h"
#include "search.h"
#include <iostream>

void TimeManager::setLimits(const SearchLimits& limits)
{
    // lol
}

Duration TimeManager::elapsed() const
{
    return std::chrono::duration_cast<Duration>(std::chrono::steady_clock::now() - m_StartTime);
}

void TimeManager::startSearch()
{
    m_StartTime = std::chrono::steady_clock::now();
}

bool TimeManager::stopHard(const SearchLimits& searchLimits) const
{
    auto time = elapsed();
    if (searchLimits.maxTime > Duration(0) && time > searchLimits.maxTime)
        return true;
    return false;
}
