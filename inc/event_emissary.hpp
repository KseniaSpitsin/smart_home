#ifndef EVENT_EMISSARY_HPP
#define EVENT_EMISSARY_HPP

#include "common_utils.hpp"
#include "runnable.hpp"

namespace smarthome
{

class IEventPackagePuller;

class EventEmissary : public advcpp::Runnable
{
public:
    EventEmissary(const SharedPtr<IEventPackagePuller>& a_puller) NOEXCEPTIONS;
    virtual ~EventEmissary() NOEXCEPTIONS;
    
    virtual void Run() NOEXCEPTIONS;
    void ShutDown() NOEXCEPTIONS;

private:
    SharedPtr<IEventPackagePuller> m_puller;
    advcpp::AtomicFlag m_isShutDown;
};

} //namespace smarthome

#endif //EVENT_EMISSARY_HPP