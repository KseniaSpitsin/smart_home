#ifndef I_REGISTRAR_HPP
#define I_REGISTRAR_HPP

#include "common_utils.hpp"
#include "event_topic.hpp"

namespace smarthome
{

class IEventHandler;

class IRegistrar : private advcpp::Uncopyable
{
public:
    //IRegistrar() = default;
    virtual ~IRegistrar() NOEXCEPTIONS;

    virtual void Registrate(const SharedPtr<IEventHandler>& a_eventHandler, const EventTopic& a_eventTopic) = 0;
    virtual void Unregister(const SharedPtr<IEventHandler>& a_eventHandler) = 0;
};

} //namespace smarthome

#endif //I_REGISTRAR_HPP