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

namespace Data
{
    struct LogPoint
    {
        LogPoint(const TimePoint &time, const float &temperature, const float &voltage, const Hardware::ThermistorModel using_thermistor);

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
        float GetPoint(const TimePoint timepoint) const;                                                                                // TODO: reload for index
        const LogPoint &AddPoint(TimePoint &time, float &temp, const float &voltage, const Hardware::ThermistorModel using_thermistor); // TODO: end memory exception handler
        std::optional<ChannelMode> GetChannelMode() const;
        void SetChannelMode(const ChannelMode new_status);
        void UpdateAllData();
        void ClearAllData();

    private:
        std::string channel_name_{"Untitled"};
        ChannelMode mode_ = ChannelMode::CHANNEL_MODE_COUNT;
        std::deque<LogPoint> data_{};
    };
} // namespace Data