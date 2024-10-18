#pragma once

#include <string>
#include <chrono>
#include <list>
#include <tuple>
#include <unordered_map>
#include <optional>
#include <algorithm>
#include <deque>

#include "timepoint.hpp"
#include "thermistors.hpp"

#ifndef LOG_POINT_STRINGS_COUNT
#define LOG_POINT_STRINGS_COUNT (18)
#endif

#ifndef LOG_STRINGS_PER_CHANNEL
#define LOG_STRINGS_PER_CHANNEL (3)
#endif

namespace Data
{
    struct LogPoint
    {
        LogPoint(const TimePoint &time, const float &temperature, const float &voltage, const Hardware::ThermistorModel using_thermistor);
        LogPoint(const LogPoint &&other_point);

        TimePoint time_;
        float temperature_;
        float voltage_;
        Hardware::ThermistorModel using_thermistor_;
    };

    enum class ChannelMode
    {
        OFFLINE,
        ONLINE,
        CHANNEL_MODE_COUNT
    };

    class Channel
    {
    public:
        Channel();
        Channel(std::string &channel_name);
        float GetPoint(const TimePoint timepoint) const;     // TODO: reload for index
        const LogPoint &AddPoint(const LogPoint &log_point); // TODO: end memory exception handler
        std::optional<ChannelMode> GetChannelMode() const;
        void SetChannelMode(const ChannelMode new_status);
        void ClearAllData();

    private:
        std::string name_{"Untitled"};
        ChannelMode mode_ = ChannelMode::CHANNEL_MODE_COUNT;
        std::deque<LogPoint> data_{};
    };
} // namespace Data