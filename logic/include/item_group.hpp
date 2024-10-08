#pragma once

#include <string>
#include <chrono>
#include <list>
#include <tuple>

using years_t = std::chrono::years;
using months_t = std::chrono::months;
using days_t = std::chrono::days;
using hours_t = std::chrono::hours;
using minutes_t = std::chrono::minutes;
using seconds_t = std::chrono::seconds;

class TimePoint
{
private:
    std::tuple<months_t, days_t, hours_t, minutes_t, seconds_t> time_point;
};

namespace Store
{
    class Channel
    {
    private:
        std::string channel_name_{""};
        // thermistor_model_t thermistor_model_{};
        std::list<std::pair<TimePoint, float>> data_list_;
    };
} // namespace Store