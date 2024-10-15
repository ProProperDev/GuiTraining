#include "../include/channel.hpp" // TODO add path in Makefile

namespace Data
{

    TimeTempPoint::TimeTempPoint(const TimePoint &time, const float temperature)
    {
        time_ = time;
        temperature_ = temperature;
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

    const TimeTempPoint &Channel::AddPoint(TimePoint &time, float &temp)
    {
        return data_.emplace_back(time, temp);
    }

    std::optional<Hardware::ThermistorModel> Channel::GetThermistorModel() const
    {
        return thermistor_model_ == Hardware::ThermistorModel::THERMISTOR_MODELS_COUNT
                   ? std::nullopt
                   : std::optional<Hardware::ThermistorModel>(thermistor_model_);
    }

    void Channel::SetThermistorModel(const Hardware::ThermistorModel thermisor_model)
    {
        thermistor_model_ = thermisor_model;
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