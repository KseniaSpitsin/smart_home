#include "agent_information.hpp"

namespace smarthome
{

AgentInformation::AgentInformation(const AgentId& a_id, const AgentDevice& a_device,
        const AgentLocation& a_location, const AgentLog& a_log, const AgentConfig& a_config)
    : m_id(a_id)
    , m_device(a_device)
    , m_location(a_location)
    , m_log(a_log)
    , m_config(a_config)
{
}

const AgentDevice& AgentInformation::GetDevice() const NOEXCEPTIONS
{
    return m_device;
}

const AgentId& AgentInformation::GetId() const NOEXCEPTIONS
{
    return m_id;
}

const AgentLocation& AgentInformation::GetLocation() const NOEXCEPTIONS
{
    return m_location;
}

const AgentLog& AgentInformation::GetLog() const NOEXCEPTIONS
{
    return m_log;
}

const AgentConfig& AgentInformation::GetConfig() const NOEXCEPTIONS
{
    return m_config;
}

} //namespace smarthome