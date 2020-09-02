#ifndef EVENT_TOPIC_HPP
#define EVENT_TOPIC_HPP

#include "common_utils.hpp"
#include "configuration_file_details.hpp"
#include "event.hpp"

namespace smarthome
{

class EventTopic
{
public:
    EventTopic(const EventType& a_eventType = ANY, const EventLocation& a_eventLocation = Location());
    //~EventTopic() = default;
    //EventTopic(const EventTopic& a_other) = default;
    //EventTopic& operator=(const EventTopic& a_other) = default;

    const EventType& GetEventType() const NOEXCEPTIONS;
    const EventLocation& GetEventLocation() const NOEXCEPTIONS;

    bool operator==(const EventTopic& a_other) const NOEXCEPTIONS;
    bool operator!=(const EventTopic& a_other) const NOEXCEPTIONS;

private:
    EventType m_type;
    EventLocation m_location;
};

} //namespace smarthome

#endif //EVENT_TOPIC_HPP