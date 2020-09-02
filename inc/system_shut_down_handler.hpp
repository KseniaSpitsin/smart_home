#ifndef SYSTEM_SHUT_DOWN_HANDLER_HPP
#define SYSTEM_SHUT_DOWN_HANDLER_HPP

#include "common_utils.hpp"
#include "i_event_handler.hpp"

namespace smarthome
{

class SystemShutDownHandler : public IEventHandler
{
public:
    //SystemShutDownHandler() = default;
    virtual ~SystemShutDownHandler() NOEXCEPTIONS;

    virtual void HandleEvent(const SharedPtr<Event>& a_event);
};

} //namespace smarthome

#endif //SYSTEM_SHUT_DOWN_HANDLER_HPP