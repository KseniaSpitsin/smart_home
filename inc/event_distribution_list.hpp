#ifndef EVENT_DISTRIBUTION_LIST_HPP
#define EVENT_DISTRIBUTION_LIST_HPP

#include <utility>
#include <vector>

#include "common_utils.hpp"
#include "i_registrar.hpp"
#include "i_search.hpp"
#include "event_topic.hpp"
#include "mutex.hpp"
#include "subscribers_list.hpp"
#include "event_distribution_list_details.hpp"

namespace smarthome
{

class IEventHandler;

class EventDistributionList : public ISearch, public IRegistrar
{
public:
    EventDistributionList();
    virtual ~EventDistributionList() NOEXCEPTIONS;

    virtual void Registrate(const SharedPtr<IEventHandler>& a_eventHandler, const EventTopic& a_eventTopic);
    virtual void Unregister(const SharedPtr<IEventHandler>& a_eventHandler);

    virtual SubscribersList SearchEventSubscribers(const EventTopic& a_eventTopic);

private:
    typedef std::tr1::unordered_map<distribution_list_details::HandlersRegistered, distribution_list_details::TopicsPermutation, distribution_list_details::HandlersRegisteredHashFunc> HandlersMap;
    typedef std::tr1::unordered_map<distribution_list_details::TopicsPermutation, SubscribersList, distribution_list_details::TopicsMapHashFunc, distribution_list_details::TopicsPermutationCompare> TopicsMap;
    typedef std::vector<EventTopic> MatchingKeysContainer;

 private:
    static MatchingKeysContainer findMatchingKeys(const EventTopic& a_eventTopic);
    static EventTopic createEventTopicPermutations(const EventTopic& a_topic, unsigned int a_permutationFlag);

private:
    void addHandler(const SharedPtr<IEventHandler>& a_eventHandler, const EventTopic& a_eventTopic);

private:
    HandlersMap m_handlers;
    TopicsMap m_topics;
    advcpp::Mutex m_mutex;
};

} //namespace smarthome

#endif //EVENT_DISTRIBUTION_LIST_HPP