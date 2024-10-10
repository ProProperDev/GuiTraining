#pragma once

#include <string>
#include <chrono>
#include <list>
#include <tuple>
#include <unordered_map>

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
        std::unordered_map<TimePoint, float> data_list_;
    };
} // namespace data
