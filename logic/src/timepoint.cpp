#include "../include/timepoint.hpp"

namespace Data
{
    bool TimePoint::operator==(const TimePoint &other) const
    {
        if (month == other.month && day == other.day && hours == other.hours && seconds == other.seconds)
        {
            return true;
        }
        return false;
    }
} // namespace Data