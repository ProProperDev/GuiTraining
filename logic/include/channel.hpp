#pragma once

#include <string>
#include <chrono>
#include <list>
#include <tuple>
#include <unordered_map>
#include <optional>
#include <algorithm>

#include "timepoint.hpp"
#include "thermistors.hpp"

namespace Data
{

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
        float GetPoint(const TimePoint timepoint) const;  // TODO: reload for index
        auto AddPoint(TimePoint &timepoint, float &temp); // TODO: end memory exception handler; reconsider auto type
        std::optional<Hardware::ThermistorModel> GetThermistorModel() const;
        void SetThermistorModel(const Hardware::ThermistorModel thermisor_model);
        std::optional<ChannelMode> GetChannelMode() const;
        void SetChannelMode(const ChannelMode new_status);
        void UpdateAllData();
        void ClearAllData();

    private:
        size_t TimePointHasher(const TimePoint timepoint);

        std::string channel_name_{"Untitled"};
        ChannelMode mode_ = ChannelMode::CHANNEL_MODE_COUNT;
        Hardware::ThermistorModel thermistor_model_ = Hardware::ThermistorModel::THERMISTOR_MODELS_COUNT;
        std::list<std::pair<TimePoint, float>> data_;
    };
} // namespace Data