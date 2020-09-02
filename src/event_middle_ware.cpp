#include "event_middle_ware.hpp"
#include "event_puller_shut_down_exception.hpp"
#include "event_pusher_shut_down_exception.hpp"

namespace smarthome
{

EventMiddleWare::EventMiddleWare(size_t a_capacity)
    : m_container(a_capacity)
{
}

EventMiddleWare::~EventMiddleWare() NOEXCEPTIONS
{
}

void EventMiddleWare::Push(const SharedPtr<Event>& a_event)
{
    try
    {
        m_container.Enqueue(a_event);
    }
    catch(const advcpp::QueueIsShutingDown&)
    {
        throw EventPusherShutDownException();
    }
}

void EventMiddleWare::Pull(SharedPtr<Event>& a_event)
{
    try
    {
        m_container.Dequeue(a_event);
    }
    catch(const advcpp::QueueIsShutingDown&)
    {
        throw EventPullerShutDownException();
    }
}

void EventMiddleWare::ShutDown() NOEXCEPTIONS
{
    m_container.ShutDown();
}

} //namespace smarthome