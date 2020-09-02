#ifndef EVENT_DISTRIBUTION_LIST_DETAILS_HPP
#define EVENT_DISTRIBUTION_LIST_DETAILS_HPP

#include "event_topic.hpp"

namespace smarthome
{

class IEventHandler;

namespace distribution_list_details
{

class TopicHashFunc
{
public:
    size_t operator()(const EventTopic& a_topic) const;
};

typedef std::tr1::unordered_set<EventTopic, TopicHashFunc> TopicsPermutation;
typedef SharedPtr<IEventHandler> HandlersRegistered;

class TopicsMapHashFunc
{
public:
    size_t operator()(const TopicsPermutation& a_topics) const;
};

class TopicsPermutationCompare
{
public:
    bool operator()(const TopicsPermutation& a_first, const TopicsPermutation& a_second) const;
};

class HandlersRegisteredHashFunc
{
public:
    size_t operator()(const SharedPtr<IEventHandler>& a_eventHandler) const;
};

} //namespace distribution_list_details

} //namespace smarthome

#endif //EVENT_DISTRIBUTION_LIST_DETAILS_HPP