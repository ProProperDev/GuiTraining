#include "../include/channel.hpp" // TODO add path in Makefile

namespace Data
{
    Channel::Channel() = default;

    Channel::Channel(std::string &channel_name)
        : channel_name_(channel_name) {};
    float Channel::GetPoint(const TimePoint timepoint) const
    {
        return data_.at(timepoint);
    }
    auto Channel::AddPoint(TimePoint &timepoint, float &temp)
    {
        data_[timepoint] = temp;
        return data_;
    }
    std::optional<Hardware::ThermistorModel_t> Channel::GetThermistorModel() const
    {
        return thermistor_model_ == Hardware::ThermistorModel_t::THERMISTOR_MODELS_COUNT
                   ? std::nullopt
                   : std::optional<Hardware::ThermistorModel_t>(thermistor_model_);
    }
    void Channel::SetThermistorModel(const Hardware::ThermistorModel_t thermisor_model)
    {
        thermistor_model_ = thermisor_model;
    }

} // namespace Data