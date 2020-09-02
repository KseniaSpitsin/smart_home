#include "hub.hpp"
#include "event_topic.hpp"
#include "distribution_info.hpp"
#include "event_puller_shut_down_exception.hpp"
#include "system_shut_down_handler.hpp"
#include "event_package.hpp"

namespace smarthome
{

Hub::Hub(const SharedPtr<IEventDistributer>& a_eventSender , const SharedPtr<IEventPuller>& a_eventPuller, const SharedPtr<ISearch>& a_subscribersSearcher) NOEXCEPTIONS
    : m_eventSender(a_eventSender)
    , m_eventPuller(a_eventPuller)
    , m_searcher(a_subscribersSearcher)
    , m_thread(new advcpp::RThread(*this))
    , m_isRunning(true)
{
}

Hub::~Hub() NOEXCEPTIONS
{
}

void Hub::Run() NOEXCEPTIONS
{
    while(m_isRunning.Get())
    {
        SharedPtr<Event> eventOut;

        try
        {
            m_eventPuller->Pull(eventOut);
            EventTopic topic(eventOut->GetType(), eventOut->GetLocation());
            SubscribersList subList = m_searcher->SearchEventSubscribers(topic);
            m_eventSender->Deliver(eventOut, subList);
        }
        catch(const EventPullerShutDownException&)
        {
            m_isRunning.Release();
        }
        catch(const std::exception& e)
        {
           continue;
        }
    }
}

void Hub::ShutDown()
{
    if(m_isRunning.Get())
    {
        m_isRunning.Release();
    }
    m_thread->Join();
    
    SharedPtr<SystemShutDownHandler> shutDownHandler(new SystemShutDownHandler());
    SharedPtr<Event> eventDummy;

    EventPackage shutDownPackage(shutDownHandler, eventDummy);

    m_eventSender->PublicAll(shutDownPackage);
}

} //namespace smarthome