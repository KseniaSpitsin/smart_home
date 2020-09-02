#ifndef SENSOR_AGENT_HPP
#define SENSOR_AGENT_HPP

#include "common_utils.hpp"
#include "agent_information.hpp"
#include "runnable.hpp"
#include "i_agent.hpp"
#include "r_thread.hpp"

namespace smarthome
{

class IEventPusher;
class IEventTypeParser;

class SensorAgent : private advcpp::Runnable, public IAgent
{
public:
    SensorAgent(const AgentInformation& a_info, const SharedPtr<IEventTypeParser>& a_parser);
    virtual ~SensorAgent() NOEXCEPTIONS;

    virtual void Connect(const SharedPtr<IRegistrar>& a_registrar, const SharedPtr<IEventPusher>& a_pusher);
    virtual void Disconnect();

private:
    virtual void Run() NOEXCEPTIONS; 

private:
    AgentInformation m_info;
    SharedPtr<IEventTypeParser> m_parser;
    SharedPtr<advcpp::RThread> m_thread;
    advcpp::AtomicFlag m_isRunning;
    SharedPtr<IEventPusher> m_pusher;
};

} //namespace smarthome

#endif //SENSOR_AGENT_HPP