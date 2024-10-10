#pragma once

#include <string>
#include <chrono>
#include <list>
#include <tuple>
#include <map>

#include "timepoint.hpp"

namespace data
{
    class Channel
    {
    public:
        float GetPoint(const TimePoint) const;

    private:
        std::string channel_name_{""};
        // thermistor_model_t thermistor_model_{};
        std::map<TimePoint, float, std::less<>> data_list_;
    };
} // namespace data