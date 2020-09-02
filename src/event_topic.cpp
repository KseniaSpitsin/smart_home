#include "event_topic.hpp"

namespace smarthome
{

EventTopic::EventTopic(const EventType& a_eventType, const EventLocation& a_eventLocation)
    : m_type(a_eventType)
    , m_location(a_eventLocation)
{
}

const EventType& EventTopic::GetEventType() const NOEXCEPTIONS
{
    return m_type;
}

const EventLocation& EventTopic::GetEventLocation() const NOEXCEPTIONS
{
    return m_location;
}

bool EventTopic::operator==(const EventTopic& a_other) const NOEXCEPTIONS
{
    return m_location == a_other.m_location && m_type == a_other.GetEventType();
}

bool EventTopic::operator!=(const EventTopic& a_other) const NOEXCEPTIONS
{
    return !(*this == a_other); 
}

} //namespace smarthome