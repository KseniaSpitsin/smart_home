#ifndef I_EVENT_PULLER_HPP
#define I_EVENT_PULLER_HPP

#include "common_utils.hpp"

namespace smarthome
{

class Event;

class IEventPuller : private advcpp::Uncopyable
{
public:
    //IEventPuller() = default;
    virtual ~IEventPuller() NOEXCEPTIONS;

    //!@exception throws EventPullerShutDownException on shut down.
    virtual void Pull(SharedPtr<Event>& a_event) = 0;
};

} //namespace smarthome

#endif //I_EVENT_PULLER_HPP