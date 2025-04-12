#include "../include/timepoint.hpp"

namespace Data
{
    TimePoint& TimePoint::operator=(const TimePoint&& other) {
        hours_ = std::move(other.hours_);
        minutes_ = std::move(other.minutes_);
        seconds_ = std::move(other.seconds_);
        return *this;
    }

    TimePoint& TimePoint::operator=(const TimePoint& other) {
        hours_ = other.hours_;
        minutes_ = other.minutes_;
        seconds_ = other.seconds_;
        return *this;
    }

    TimePoint::TimePoint(const TimePoint&& other) {
        hours_ = std::move(other.hours_);
        minutes_ = std::move(other.minutes_);
        seconds_ = std::move(other.seconds_);
    }

    TimePoint::TimePoint(const TimePoint& other) {
        hours_ = other.hours_;
        minutes_ = other.minutes_;
        seconds_ = other.seconds_;
    }

    TimePoint::TimePoint(const hours_t &hours, const minutes_t &minutes, const seconds_t &seconds)
    {
        hours_ = hours;
        minutes_ = minutes;
        seconds_ = seconds;
    }

    bool TimePoint::operator==(const TimePoint &other) const
    {
        if (hours_ == other.hours_ && minutes_ == other.minutes_ && seconds_ == other.seconds_)
        {
            return true;
        }
        return false;
    }

    bool TimePoint::operator<(const TimePoint &other) const
    {
        if (hours_ < other.hours_ && minutes_ < other.minutes_ && seconds_ < other.seconds_)
        {
            return true;
        }
        return false;
    }

    std::ostream &operator<<(std::ostream &out, const TimePoint &timepoint)
    {
        out << timepoint.hours_.count() << ":" << timepoint.minutes_.count() << ":" << timepoint.seconds_.count() << " ";
        return out;
    }

} // namespace Data