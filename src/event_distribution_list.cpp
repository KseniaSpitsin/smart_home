#include <cassert>
#include <iostream>
#include "event_distribution_list.hpp"
#include "location.hpp"
#include "event.hpp"
#include "distribution_info.hpp"
#include "configuration_file_details.hpp"

namespace smarthome
{

static const unsigned int NUM_OF_KEY_OPTIONS = 4;

EventDistributionList::EventDistributionList()
    : m_handlers()
    , m_topics()
    , m_mutex()
{
}

EventDistributionList::~EventDistributionList() NOEXCEPTIONS
{
}

void EventDistributionList::Registrate(const SharedPtr<IEventHandler>& a_eventHandler, const EventTopic& a_eventTopic)
{
    advcpp::LockGuard _(m_mutex);
    HandlersMap::iterator foundHandlerItr = m_handlers.find(a_eventHandler);
    if(m_handlers.end() == foundHandlerItr)
    {
        addHandler(a_eventHandler, a_eventTopic);
    }
    else
    {
        distribution_list_details::TopicsPermutation& handlersEventTopics = foundHandlerItr->second;
        TopicsMap::iterator foundTopicItr = m_topics.find(handlersEventTopics);
        SubscribersList& topicsSubscribersList = foundTopicItr->second;
        SubscribersListItr subscribersItr = topicsSubscribersList.Find(a_eventHandler);
        DistributionInfo subscriberInfoCopy(*subscribersItr);
        topicsSubscribersList.Erase(subscribersItr);
        m_handlers[a_eventHandler].insert(a_eventTopic);
        foundHandlerItr = m_handlers.find(a_eventHandler);
        m_topics[foundHandlerItr->second].Add(subscriberInfoCopy);
    }
}

void EventDistributionList::Unregister(const SharedPtr<IEventHandler>& a_eventHandler)
{
    advcpp::LockGuard _(m_mutex);
    HandlersMap::iterator foundHandlerItr = m_handlers.find(a_eventHandler);

    if(m_handlers.end() == foundHandlerItr)
    {
        return;
    }

    distribution_list_details::TopicsPermutation& handlersEventTopics = foundHandlerItr->second;
    SubscribersListItr subscribersList = m_topics[handlersEventTopics].Find(a_eventHandler);
    m_topics[handlersEventTopics].Erase(subscribersList);
    m_handlers.erase(a_eventHandler);
}

SubscribersList EventDistributionList::SearchEventSubscribers(const EventTopic& a_eventTopic)
{
    MatchingKeysContainer relevantKeys = findMatchingKeys(a_eventTopic);
    SubscribersList eventSubscribers;
    advcpp::LockGuard _(m_mutex);

    for(size_t i = 0; i < NUM_OF_KEY_OPTIONS; ++i)
    {
        typename TopicsMap::iterator topicsMapItr = m_topics.begin();
        typename TopicsMap::iterator endTopicsMap = m_topics.end();

        while(topicsMapItr != endTopicsMap)
        {
            distribution_list_details::TopicsPermutation::const_iterator foundItr = topicsMapItr->first.find(relevantKeys[i]);
            if(foundItr != topicsMapItr->first.end())
            {
                eventSubscribers.Add(topicsMapItr->second.begin(), topicsMapItr->second.end());
            }
            ++topicsMapItr;
        }
    }

    return eventSubscribers;
}

EventDistributionList::MatchingKeysContainer EventDistributionList::findMatchingKeys(const EventTopic& a_eventTopic)
{
    MatchingKeysContainer keys;
    keys.reserve(NUM_OF_KEY_OPTIONS);

    for(size_t i = 0; i < NUM_OF_KEY_OPTIONS; ++i)
    {
        keys.push_back(createEventTopicPermutations(a_eventTopic, i));
    }

    return keys;
}

EventTopic EventDistributionList::createEventTopicPermutations(const EventTopic& a_topic, unsigned int a_permutationFlag)
{
    assert(a_permutationFlag < NUM_OF_KEY_OPTIONS);
    const EventType& eventType = a_topic.GetEventType();
    const EventLocation& eventLocation = a_topic.GetEventLocation();
    const EventLocation::FloorType& locationFloor = eventLocation.GetFloor();

    switch(a_permutationFlag)
    {
        case 0:
            return a_topic;
            break;
        case 1:
            return EventTopic(eventType, EventLocation(locationFloor));
            break;
        case 2:
            return EventTopic(eventType, EventLocation());
            break;
        default:
            break;
    }

    return EventTopic();
}

void EventDistributionList::addHandler(const SharedPtr<IEventHandler>& a_eventHandler, const EventTopic& a_eventTopic)
{
    m_handlers[a_eventHandler].insert(a_eventTopic);
    typename HandlersMap::iterator handlerItr = m_handlers.find(a_eventHandler);
    distribution_list_details::TopicsPermutation& handlersTopics = handlerItr->second;
    m_topics[handlersTopics].Add(DistributionInfo(a_eventHandler));
}

} //namespace smarthome