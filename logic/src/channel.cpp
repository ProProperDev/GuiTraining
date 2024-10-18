#include "../include/channel.hpp"

namespace Data
{

    LogPoint::LogPoint(const TimePoint &time, const float &temperature, const float &voltage, const Hardware::ThermistorModel using_thermistor)
    {
        time_ = time;
        temperature_ = temperature;
        voltage_ = voltage;
        using_thermistor_ = using_thermistor;
    }

    LogPoint::LogPoint(const LogPoint &&other_point)
    {
        time_ = std::move(other_point.time_);
        temperature_ = std::move(other_point.temperature_);
        voltage_ = std::move(other_point.voltage_);
        using_thermistor_ = std::move(other_point.using_thermistor_);
    }

    Channel::Channel() = default;

    Channel::Channel(std::string &channel_name)
        : name_(std::move(channel_name)) {};
    float Channel::GetPoint(const TimePoint timepoint) const
    {
        auto it = std::find_if(data_.begin(), data_.end(), [&timepoint](auto &point)
                               { return point.time_ == timepoint; });
        return it->temperature_;
    }

    const LogPoint &Channel::AddPoint(const LogPoint &log_point)
    {
        return data_.emplace_back(std::move(log_point));
    }

    std::optional<ChannelMode> Channel::GetChannelMode() const
    {
        return mode_ == ChannelMode::CHANNEL_MODE_COUNT ? std::nullopt : std::optional<ChannelMode>(mode_);
    }

    void Channel::SetChannelMode(const ChannelMode new_mode)
    {
        mode_ = new_mode;
    }

    void Channel::ClearAllData()
    {
        data_.clear();
        name_ = "Untitled";
        mode_ = ChannelMode::CHANNEL_MODE_COUNT;
    }

} // namespace Data