#include "distribution_info.hpp"
#include "i_event_handler.hpp"

namespace smarthome
{

DistributionInfo::ChannelId DistributionInfo::s_id = 0;

DistributionInfo::ChannelId DistributionInfo::CalcId()
{
    ChannelId currentId = s_id;
    s_id = (s_id + 1) % NUM_OF_CHANNELS;
    return currentId;
}

DistributionInfo::DistributionInfo(const SharedPtr<IEventHandler>& a_eventHandler) NOEXCEPTIONS
    : m_handler(a_eventHandler)
    , m_id(CalcId())
{
}

SharedPtr<IEventHandler> DistributionInfo::GetEventHandler() NOEXCEPTIONS
{
    return m_handler;
}

DistributionInfo::ChannelId DistributionInfo::GetChannelId() const NOEXCEPTIONS
{
    return m_id;
}

bool DistributionInfo::operator==(const DistributionInfo& a_other) const NOEXCEPTIONS
{
    return m_handler == a_other.m_handler && m_id == a_other.m_id;
}

bool DistributionInfo::operator!=(const DistributionInfo& a_other) const NOEXCEPTIONS
{
    return !(*this == a_other);
}

} //namespace smarthome