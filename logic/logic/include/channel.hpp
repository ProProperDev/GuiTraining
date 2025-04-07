#pragma once

#include <string>
#include <chrono>
#include <list>
#include <tuple>
#include <unordered_map>
#include <optional>
#include <algorithm>
#include <deque>
#include <map>

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
        LogPoint(const TimePoint &time, const float &temperature, const int voltage, const Hardware::ThermistorModel using_thermistor);
        LogPoint(const LogPoint &&other_point);
        LogPoint(const TimePoint &time);
        LogPoint(const LogPoint &other);
        const LogPoint &operator=(const LogPoint &other);
        void SetVoltage(const int voltage);
        void SetTemperature(const float temperature);
        void SetThermistor(const Hardware::ThermistorModel using_thermistor);

        const TimePoint &GetTime() const;

        TimePoint time_;
        float temperature_;
        int voltage_;
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
        Channel(std::string channel_name);
        std::string GetChannelName() const;
        const LogPoint &AddPoint(const LogPoint &log_point); // TODO: end memory exception handler
        std::optional<ChannelMode> GetChannelMode() const;
        std::optional<LogPoint *> GetLogPoint(const TimePoint &timepoint) const;
        void SetChannelMode(const ChannelMode new_status);
        const std::deque<LogPoint> &GetData() const;
        void ClearAllData();

    private:
        std::string name_{"Untitled"};
        ChannelMode mode_ = ChannelMode::CHANNEL_MODE_COUNT;
        std::map<const TimePoint, LogPoint *> timepoint_to_logpoint_{};
        std::deque<LogPoint> data_{};
    };
} // namespace Data
