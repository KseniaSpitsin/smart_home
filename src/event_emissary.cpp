#include "event_emissary.hpp"
#include "event_package.hpp"
#include "i_event_package_puller.hpp"
#include "event_package_puller_shut_down_exception.hpp"
#include "i_event_handler.hpp"
#include "system_shut_down_exception.hpp"

namespace smarthome
{

EventEmissary::EventEmissary(const SharedPtr<IEventPackagePuller>& a_puller) NOEXCEPTIONS
    : m_puller(a_puller)
    , m_isShutDown()
{
}

EventEmissary::~EventEmissary() NOEXCEPTIONS
{
}

void EventEmissary::Run() NOEXCEPTIONS
{
    while(!m_isShutDown.Get())
    {
        EventPackage package;
        try
        {
            m_puller->Pull(package);
            SharedPtr<IEventHandler>& eventhandler = package.first;
            SharedPtr<Event>& eventReceived = package.second;
            eventhandler->HandleEvent(eventReceived);
        }
        catch(const EventPackagePullerShutDownException&)
        {
            m_isShutDown.Acquire();
        }
        catch(const SystemShutDownException&)
        {
            m_isShutDown.Acquire();
        }
        catch(const std::exception& e)
        {
            continue;
        }
    }
}

void EventEmissary::ShutDown() NOEXCEPTIONS
{
    if(!m_isShutDown.Get())
    {
        m_isShutDown.Acquire();
    }
}

} //namespace smarthome