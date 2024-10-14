#pragma once

#include <chrono>
#include <tuple>

using years_t = std::chrono::years;
using months_t = std::chrono::months;
using days_t = std::chrono::days;
using hours_t = std::chrono::hours;
using minutes_t = std::chrono::minutes;
using seconds_t = std::chrono::seconds;

namespace Data
{
    struct TimePoint
    {
        TimePoint(std::string date_and_time_str);
        bool operator==(const TimePoint &other) const;

        years_t year{};
        months_t month{};
        days_t day{};
        hours_t hours{};
        minutes_t minutes{};
        seconds_t seconds{};
    };
} // namespace Data