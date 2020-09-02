#include "subscribers_list_itr.hpp"
#include "distribution_info.hpp"

namespace smarthome
{

SubscribersListItr::SubscribersListItr(Itr a_listItr) NOEXCEPTIONS
    : m_listItr(a_listItr)
{
}

SubscribersListItr& SubscribersListItr::operator++() NOEXCEPTIONS
{
    ++m_listItr;
    return *this;
}

bool SubscribersListItr::operator==(const SubscribersListItr& a_other) const NOEXCEPTIONS
{
    return m_listItr == a_other.m_listItr;
}

bool SubscribersListItr::operator!=(const SubscribersListItr& a_other) const NOEXCEPTIONS
{
    return !(*this == a_other);
}

DistributionInfo& SubscribersListItr::operator*() NOEXCEPTIONS
{
    return *m_listItr;
}

DistributionInfo* SubscribersListItr::operator->() NOEXCEPTIONS
{
    return &(*m_listItr);
}


} //namespace smarthome