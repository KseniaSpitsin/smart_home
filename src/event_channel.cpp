#include "event_channel.hpp"
#include "event_channel_configuration.hpp"
#include "queue_exceptions.hpp"
#include "event_package_puller_shut_down_exception.hpp"
#include "event_package_pusher_shut_down_exception.hpp"

namespace smarthome
{

EventChannel::EventChannel()
    : m_eventChannel(EVENT_CHANNEL_CAPACITY)
{
}

EventChannel::~EventChannel() NOEXCEPTIONS
{
}

void EventChannel::Push(const EventPackage& a_info)
{
    try
    {
        m_eventChannel.Enqueue(a_info);
    }
    catch(const advcpp::QueueIsShutingDown&)
    {
        throw EventPackagePusherShutDownException();
    }
}

void EventChannel::Pull(EventPackage& a_info)
{
    try
    {
        m_eventChannel.Dequeue(a_info);
    }
    catch(const advcpp::QueueIsShutingDown&)
    {
        throw EventPackagePullerShutDownException();
    }
}

} //namespace smarthome