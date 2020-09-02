#ifndef DISTRIBUTION_INFO_HPP
#define DISTRIBUTION_INFO_HPP

#include "common_utils.hpp"
#include "event_channel_configuration.hpp"

namespace smarthome
{

class IEventHandler;

class DistributionInfo
{
public:
    typedef unsigned int ChannelId;

public:
    explicit DistributionInfo(const SharedPtr<IEventHandler>& a_eventHandler) NOEXCEPTIONS;
    //~DistributionInfo() = default;
    //DistributionInfo(const DistributionInfo& a_other) = default;
    //DistributinList& operator=(const DistributionList& a+_other) = default;

    SharedPtr<IEventHandler> GetEventHandler() NOEXCEPTIONS;
    ChannelId GetChannelId() const NOEXCEPTIONS;

    bool operator==(const DistributionInfo& a_other) const NOEXCEPTIONS;
    bool operator!=(const DistributionInfo& a_other) const NOEXCEPTIONS;

private:
    static ChannelId CalcId();

private:
    static ChannelId s_id;

private:
    SharedPtr<IEventHandler> m_handler;
    ChannelId m_id;
};

} //namespace smarthome

#endif //DISTRIBUTION_INFO_HPP