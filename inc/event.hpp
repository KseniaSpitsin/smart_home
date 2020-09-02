#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>
#include <utility>

#include "common_utils.hpp"
#include "location.hpp"

namespace smarthome
{

typedef std::string EventTime;
typedef std::string EventData;
typedef std::string EventType;
typedef Location EventLocation;

class Event
{
public:
    Event(const EventType& a_type, const EventLocation& a_location, const EventTime& a_time, const EventData& a_data = "");
    //~Event() = default;
    //Event(const Event& a_other) = default;
    //Event& operator=(const Event& a_other) = default;

    const EventType& GetType() const;
    const EventLocation& GetLocation() const;
    const EventTime& GetTime() const;
    const EventData& GetData() const;
    
    bool operator==(const Event& a_other) const;
    bool operator!=(const Event& a_other) const;

private:
    EventType m_type;
    EventLocation m_location;
    EventTime m_time;
    EventData m_data;
};

} //namespace smarthome

#endif // EVENT_HPP