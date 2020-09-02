#ifndef AGENT_INFORMATION_HPP
#define AGENT_INFORMATION_HPP

#include <string>
#include "common_utils.hpp"
#include "location.hpp"

namespace smarthome
{

typedef std::string AgentId;
typedef std::string AgentDevice;
typedef Location AgentLocation;
typedef std::string AgentLog;
typedef std::string AgentConfig;

class AgentInformation : private advcpp::Unassignable
{
public:
    AgentInformation(const AgentId& a_id, const AgentDevice& a_device, const AgentLocation& a_location, const AgentLog& a_log = "", const AgentConfig& a_config = "");
    //~AgentInformation() = default;
    //AgentInformation(const AgentInformation& a_other) = default

    const AgentId& GetId() const NOEXCEPTIONS;
    const AgentDevice& GetDevice() const NOEXCEPTIONS;
    const AgentLocation& GetLocation() const NOEXCEPTIONS;
    const AgentLog& GetLog() const NOEXCEPTIONS;
    const AgentConfig& GetConfig() const NOEXCEPTIONS;

private:
    AgentId m_id;
    AgentDevice m_device;
    AgentLocation m_location;
    AgentLog m_log;
    AgentConfig m_config;
};

} //namespace smarthome

#endif //AGENT_INFORMATION_HPP