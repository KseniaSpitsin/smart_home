#include "event.hpp"

namespace smarthome
{

Event::Event(const EventType& a_type, const EventLocation& a_location, const EventTime& a_time, const EventData& a_data)
    : m_type(a_type)
    , m_location(a_location)
    , m_time(a_time)
    , m_data(a_data)
{
}

const EventType& Event::GetType() const
{
    return m_type;
}

const EventLocation& Event::GetLocation() const
{
    return m_location;
}

const EventTime& Event::GetTime() const
{
    return m_time;
}

const EventData& Event::GetData() const
{
    return m_data;
}

bool Event::operator==(const Event& a_other) const
{
    return m_type == a_other.m_type && m_location == a_other.m_location && m_time == a_other.m_time
        && m_data == a_other.m_data;
}

bool Event::operator!=(const Event& a_other) const
{
    return !(*this == a_other);
}

} //namespace smarthome