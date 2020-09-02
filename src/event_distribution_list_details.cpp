#include <functional>
#include <algorithm>
#include "event_distribution_list_details.hpp"
#include "i_event_handler.hpp"

namespace smarthome
{

namespace distribution_list_details
{

size_t TopicHashFunc::operator()(const EventTopic& a_topic) const
{
    std::tr1::hash<std::string> hashFunc;
    const EventType& topicType = a_topic.GetEventType();
    const EventLocation& topicLocation = a_topic.GetEventLocation();
    return hashFunc(topicType) + hashFunc(topicLocation.GetRoom()) + hashFunc(topicLocation.GetFloor());
}

size_t TopicsMapHashFunc::operator()(const TopicsPermutation& a_topics) const
{
    size_t key = 0;
    TopicHashFunc topicHash;
    TopicsPermutation::const_iterator begin = a_topics.begin();
    TopicsPermutation::const_iterator end = a_topics.end();

    while(begin != end)
    {
        key ^= topicHash(*begin);
        ++begin; 
    }

    return key;
}

bool TopicsPermutationCompare::operator()(const TopicsPermutation& a_first, const TopicsPermutation& a_second) const
{
    TopicsPermutation::const_iterator itr = a_first.begin();
    TopicsPermutation::const_iterator last = a_first.end();

    if(a_first.size() != a_second.size())
    {
        return false;
    }

    while(itr != last)
    {
        if(a_second.find(*itr) == a_second.end())
        {
            return false;
        }

        ++itr;
    }

    return true;
}

size_t HandlersRegisteredHashFunc::operator()(const SharedPtr<IEventHandler>& a_eventHandler) const
{
    std::tr1::hash<IEventHandler*> hashFunc;
    return hashFunc(a_eventHandler.get());
}

} //namespace distribution_list_details

} //namespace smarthome