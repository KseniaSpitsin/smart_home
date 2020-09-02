#ifndef TEST_SENSOR_HPP
#define TEST_SENSOR_HPP

#include "common_utils.hpp"
#include "agent_information.hpp"
#include "runnable.hpp"
#include "i_agent.hpp"
#include "r_thread.hpp"

namespace smarthome
{

class IEventPusher;
class IEventTypeParser;

class TestSensor : private advcpp::Runnable, public IAgent
{
public:
    TestSensor(const AgentInformation& a_info, const SharedPtr<IEventTypeParser>& a_parser, size_t a_numOfEvents);
    virtual ~TestSensor() NOEXCEPTIONS;

    virtual void Connect(const SharedPtr<IRegistrar>& a_registrar, const SharedPtr<IEventPusher>& a_pusher);
    virtual void Disconnect();

private:
    virtual void Run() NOEXCEPTIONS; 

private:
    AgentInformation m_info;
    size_t m_numOfEvents;
    SharedPtr<IEventTypeParser> m_parser;
    SharedPtr<IEventPusher> m_pusher;
    SharedPtr<advcpp::RThread> m_thread;
};

} //namespace smarthome

#endif //TEST_SENSOR_HPP