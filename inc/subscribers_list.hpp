#ifndef SUBSCRIBERS_LIST_HPP
#define SUBSCRIBERS_LIST_HPP

#include <vector>
#include "subscribers_list_itr.hpp"

namespace smarthome
{

class DistributionInfo;
class IEventHandler;

class SubscribersList
{
public:
    SubscribersList();
    //~SubscribersList() = default;
    //SubscribersList(const SubscribersList& a_other) = default;
    //SubscribersList& operator=(const SubscribersList& a_other) = default;

    void Add(const DistributionInfo& a_distributionInfo);
    void Add(SubscribersListItr a_first, SubscribersListItr a_last);

    void Erase(SubscribersListItr a_itr) NOEXCEPTIONS;

    size_t Size() const NOEXCEPTIONS;

    SubscribersListItr begin() NOEXCEPTIONS;
    SubscribersListItr end() NOEXCEPTIONS;

    SubscribersListItr Find(const SharedPtr<IEventHandler>& a_eventHandler) NOEXCEPTIONS;

private:
    typedef std::vector<DistributionInfo> List;

private:
    List m_subscribers;
};

} //namespace smarthome

#endif //SUBSCRIBERS_LIST_HPP