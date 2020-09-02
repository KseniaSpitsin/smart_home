#include "event_disseminator.hpp"
#include "subscribers_list.hpp"
#include "event.hpp"
#include "i_event_package_pusher.hpp"
#include "distribution_info.hpp"
#include "event_package_pusher_shut_down_exception.hpp"

namespace smarthome
{

EventDisseminator::EventDisseminator(const Channels& a_channels)
    : m_channels(a_channels)
{
}

EventDisseminator::~EventDisseminator() NOEXCEPTIONS
{
}

void EventDisseminator::Deliver(const SharedPtr<Event>& a_event, SubscribersList a_list) NOEXCEPTIONS
{
    SubscribersListItr listBegin = a_list.begin();
    SubscribersListItr listEnd = a_list.end();

    while(listBegin != listEnd)
    {
        try
        {
            EventPackage package(listBegin->GetEventHandler(), a_event);
            m_channels[listBegin->GetChannelId()]->Push(package);
        }
        catch(const EventPackagePusherShutDownException&)
        {
            break;
        }
        catch(const std::exception&)
        {
            ++listBegin;
            continue;
        }

        ++listBegin;
    }
}

void EventDisseminator::PublicAll(const EventPackage& a_package)
{
    size_t maxChannels = m_channels.size();

    for(size_t i = 0; i < maxChannels; ++i)
    {
        m_channels[i]->Push(a_package);
    }
}

} //namespace smarthome