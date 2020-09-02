#include <algorithm>
#include "subscribers_list.hpp"
#include "distribution_info.hpp"

namespace smarthome
{

SubscribersList::SubscribersList()
    : m_subscribers()
{
}

void SubscribersList::Add(const DistributionInfo& a_distributionInfo)
{
    m_subscribers.push_back(a_distributionInfo);
}

size_t SubscribersList::Size() const NOEXCEPTIONS
{
    return m_subscribers.size();
}

void SubscribersList::Add(SubscribersListItr a_first, SubscribersListItr a_last)
{
    while(a_first != a_last)
    {
        m_subscribers.push_back(*a_first);
        ++a_first;
    }
}

void SubscribersList::Erase(SubscribersListItr a_itr) NOEXCEPTIONS
{
    m_subscribers.erase(a_itr.m_listItr);
}

SubscribersListItr SubscribersList::begin() NOEXCEPTIONS
{
    return m_subscribers.begin();
}

SubscribersListItr SubscribersList::end() NOEXCEPTIONS
{
    return m_subscribers.end();
}

SubscribersListItr SubscribersList::Find(const SharedPtr<IEventHandler>& a_eventHandler) NOEXCEPTIONS
{
    SubscribersListItr first = begin();
    SubscribersListItr last = end();

    while(first != last)
    {
        if(a_eventHandler == first->GetEventHandler())
        {
            break;
        }

        ++first;
    }

    return first;
}

} //namespace smarthome