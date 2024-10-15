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
    struct TimeTempPoint
    {
        TimeTempPoint(const TimePoint &time, const float temperature);

        TimePoint time_;
        float temperature_;
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
        float GetPoint(const TimePoint timepoint) const;             // TODO: reload for index
        const TimeTempPoint &AddPoint(TimePoint &time, float &temp); // TODO: end memory exception handler; reconsider auto type
        std::optional<Hardware::ThermistorModel> GetThermistorModel() const;
        void SetThermistorModel(const Hardware::ThermistorModel thermisor_model);
        std::optional<ChannelMode> GetChannelMode() const;
        void SetChannelMode(const ChannelMode new_status);
        void UpdateAllData();
        void ClearAllData();

    private:
        std::string channel_name_{"Untitled"};
        ChannelMode mode_ = ChannelMode::CHANNEL_MODE_COUNT;
        Hardware::ThermistorModel thermistor_model_ = Hardware::ThermistorModel::THERMISTOR_MODELS_COUNT;
        std::deque<const TimeTempPoint> data_{};
    };
} // namespace Data