#pragma once

#include <chrono>
#include <array>
#include "defs.h"

using TimePoint = std::chrono::steady_clock::time_point;
using Duration = std::chrono::milliseconds;

struct SearchLimits
{
    int maxDepth;
    Duration maxTime;
};

class TimeManager
{
public:
    TimeManager() = default;

    void setLimits(const SearchLimits& searchLimits);
    Duration elapsed() const;

    void startSearch();
    bool stopHard(const SearchLimits& searchLimits) const;
private:
    TimePoint m_StartTime;
};