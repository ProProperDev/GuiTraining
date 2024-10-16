#include "../include/channel.hpp" // TODO add path in Makefile

namespace Data
{

    LogPoint::LogPoint(const TimePoint &time, const float &temperature, const float &voltage, const Hardware::ThermistorModel using_thermistor)
    {
        time_ = time;
        temperature_ = temperature;
        voltage_ = voltage;
        using_thermistor_ = using_thermistor;
    }

    Channel::Channel() = default;

    Channel::Channel(std::string &channel_name)
        : channel_name_(std::move(channel_name)) {};
    float Channel::GetPoint(const TimePoint timepoint) const
    {
        auto it = std::find_if(data_.begin(), data_.end(), [&timepoint](auto &point)
                               { return point.time_ == timepoint; });
        return it->temperature_;
    }

    const LogPoint &Channel::AddPoint(TimePoint &time, float &temp, const float &voltage, const Hardware::ThermistorModel using_thermistor)
    {
        return data_.emplace_back(time, temp, voltage, using_thermistor);
    }

    std::optional<ChannelMode> Channel::GetChannelMode() const
    {
        return mode_ == ChannelMode::CHANNEL_MODE_COUNT ? std::nullopt : std::optional<ChannelMode>(mode_);
    }

    void Channel::SetChannelMode(const ChannelMode new_mode)
    {
        mode_ = new_mode;
    }

} // namespace Data