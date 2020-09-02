#include "test_hub.hpp"
#include "event_topic.hpp"
#include "distribution_info.hpp"
#include "event_puller_shut_down_exception.hpp"
#include "system_shut_down_handler.hpp"
#include "event_package.hpp"
#include "i_event_puller.hpp"
#include "subscribers_list.hpp"
#include "i_search.hpp"
#include "i_event_distributer.hpp"

namespace smarthome
{

TestHub::TestHub(const SharedPtr<IEventDistributer>& a_eventSender, const SharedPtr<IEventPuller>& a_eventPuller, const SharedPtr<ISearch>& a_subscribersSearcher, size_t a_eventNum)
    : m_eventSender(a_eventSender)
    , m_eventPuller(a_eventPuller)
    , m_searcher(a_subscribersSearcher)
    , m_numOfEvents(a_eventNum)
{
}

TestHub::~TestHub() NOEXCEPTIONS
{
}

void TestHub::Run() NOEXCEPTIONS
{
    for(size_t i = 0; i < m_numOfEvents; ++i)
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
            break;
        }
        catch(const std::exception& e)
        {
           continue;
        }
    }
}

void TestHub::ShutDown()
{
    SharedPtr<SystemShutDownHandler> shutDownHandler(new SystemShutDownHandler());
    SharedPtr<Event> eventDummy;

    EventPackage shutDownPackage(shutDownHandler, eventDummy);

    m_eventSender->PublicAll(shutDownPackage);
}

} //namespace smarthome