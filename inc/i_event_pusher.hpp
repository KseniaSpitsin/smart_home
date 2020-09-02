#ifndef I_EVENT_PUSHER_HPP
#define I_EVENT_PUSHER_HPP

#include "common_utils.hpp"

namespace smarthome
{

class Event;

class IEventPusher : private advcpp::Uncopyable
{
public:
    //IEventPusher() = default;
    virtual ~IEventPusher() NOEXCEPTIONS;

    //!@exception throws EventPusherShutDownException on shut down.
    virtual void Push(const SharedPtr<Event>& a_event) = 0;
};

} //namespace advcpp

#endif //I_EVENT_PUSHER_HPP