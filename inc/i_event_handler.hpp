#ifndef I_EVENT_HANDLER_HPP
#define I_EVENT_HANDLER_HPP

#include "common_utils.hpp"
#include "event.hpp"

namespace smarthome
{

class IEventHandler : private advcpp::Uncopyable
{
public:
    virtual ~IEventHandler() NOEXCEPTIONS;

    virtual void HandleEvent(const SharedPtr<Event>& a_event) = 0;
};

inline IEventHandler::~IEventHandler() NOEXCEPTIONS
{
}

} //namespace smarthome

#endif //I_EVENT_HANDLER_HPP