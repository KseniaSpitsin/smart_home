#ifndef I_SEARCH_HPP
#define I_SEARCH_HPP

#include <list>
#include "common_utils.hpp"
#include "event_topic.hpp"
#include "subscribers_list.hpp"

namespace smarthome
{

class IEventHandler;

class ISearch : private advcpp::Uncopyable
{
public:
    virtual ~ISearch() NOEXCEPTIONS;

    virtual SubscribersList SearchEventSubscribers(const EventTopic& a_eventTopic) = 0;
}; 

inline ISearch::~ISearch() NOEXCEPTIONS
{
}

} //namespace smarthome

#endif //I_SEARCH_HPP