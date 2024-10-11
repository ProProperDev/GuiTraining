#pragma once

#include <string>
#include <chrono>
#include <list>
#include <tuple>
#include <unordered_map>
#include <optional>

#include "timepoint.hpp"
#include "thermistors.hpp"

namespace Data
{

    template <typename T, typename N>
    struct TimePointHasher
    {
        size_t operator()(const T timepoint) const
        {

            return year_hasher_(timepoint.day); // year_hasher_(timepoint.year) ^ month_hasher_(timepoint.month) ^ day_hasher_(timepoint.day) ^ hour_hasher_(timepoint.hours) ^ minute_hasher_(timepoint.minutes) ^ second_hasher_(timepoint.seconds);
        }

        std::hash<N> year_hasher_;
        // std::hash<months_t> month_hasher_;
        // std::hash<days_t> day_hasher_;
        // std::hash<hours_t> hour_hasher_;
        // std::hash<minutes_t> minute_hasher_;
        // std::hash<seconds_t> second_hasher_;
    };

    class Channel
    {
    public:
        Channel();
        Channel(std::string &channel_name);
        float GetPoint(const TimePoint timepoint) const;  // TODO: reload for index
        auto AddPoint(TimePoint &timepoint, float &temp); // TODO: end memory exception handler; reconsider auto type
        std::optional<Hardware::ThermistorModel_t> GetThermistorModel() const;
        void SetThermistorModel(const Hardware::ThermistorModel_t thermisor_model);
        void UpdateAllData();
        void ClearAllData();

    private:
        std::string channel_name_{"Untitled"};
        Hardware::ThermistorModel_t thermistor_model_ = Hardware::ThermistorModel_t::THERMISTOR_MODELS_COUNT;
        std::unordered_map<TimePoint, float, TimePointHasher<TimePoint, days_t>> data_;
    };
} // namespace Data