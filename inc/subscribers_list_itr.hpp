#ifndef SUBSCRIBERS_LIST_ITR_HPP
#define SUBSCRIBERS_LIST_ITR_HPP

#include <vector>
#include "common_utils.hpp"

namespace smarthome
{

class DistributionInfo;

class SubscribersListItr
{
public:
    //~SubscribersListItr() = default;
    //SubscribersListItr(const SubscribersListItr& a_other) = default;
    //SubscribersListItr& operator=(const SubscribersListItr& a_other) = default;

    SubscribersListItr& operator++() NOEXCEPTIONS;
    bool operator==(const SubscribersListItr& a_other) const NOEXCEPTIONS;
    bool operator!=(const SubscribersListItr& a_other) const NOEXCEPTIONS;

    DistributionInfo& operator*() NOEXCEPTIONS;
    DistributionInfo* operator->() NOEXCEPTIONS;

private:
    friend class SubscribersList;
    typedef std::vector<DistributionInfo>::iterator Itr;

private:
    SubscribersListItr(Itr a_listItr) NOEXCEPTIONS;

private:
    Itr m_listItr;
};

} //namespace smarthome

#endif //SUBSCRIBERS_LIST_ITR_HPP