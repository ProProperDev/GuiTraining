#include "../include/channel.hpp"

namespace Data
{

    LogPoint::LogPoint(const TimePoint &time, const float &temperature, const int voltage, const Hardware::ThermistorModel using_thermistor)
    {
        time_ = time;
        temperature_ = temperature;
        voltage_ = voltage;
        using_thermistor_ = using_thermistor;
    }

    LogPoint::LogPoint(const LogPoint &other)
    {
        time_ = other.time_;
        temperature_ = other.temperature_;
        voltage_ = other.voltage_;
        using_thermistor_ = other.using_thermistor_;
    }

    const LogPoint &LogPoint::operator=(const LogPoint &other)
    {
        time_ = other.time_;
        temperature_ = other.temperature_;
        voltage_ = other.voltage_;
        using_thermistor_ = other.using_thermistor_;
        return *this;
    }

    LogPoint::LogPoint(const TimePoint &time)
    {
        time_ = time;
    }

    void LogPoint::SetVoltage(const int voltage)
    {
        voltage_ = voltage;
    }
    void LogPoint::SetTemperature(const float temperature)
    {
        temperature_ = temperature;
    }
    void LogPoint::SetThermistor(const Hardware::ThermistorModel using_thermistor)
    {
        using_thermistor_ = using_thermistor;
    }

    const TimePoint &LogPoint::GetTime() const
    {
        return time_;
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

    const LogPoint &Channel::AddPoint(const LogPoint &log_point)
    {
        auto &emplaced_logpoint = data_.emplace_back(std::move(log_point));
        timepoint_to_logpoint_[log_point.GetTime()] = &emplaced_logpoint;
        return emplaced_logpoint;
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
        timepoint_to_logpoint_.clear();
        name_ = "Untitled";
        mode_ = ChannelMode::CHANNEL_MODE_COUNT;
    }

    std::optional<LogPoint *> Channel::GetLogPoint(const TimePoint &timepoint) const
    {
        try
        {
            return timepoint_to_logpoint_.at(timepoint);
        }
        catch (const std::out_of_range &e)
        {
            return std::nullopt;
        }
    }

} // namespace Data
