#ifndef CONTROLLER_AGENT_HPP
#define CONTROLLER_AGENT_HPP

#include <vector>
#include <fstream>

#include "i_event_handler.hpp"
#include "i_agent.hpp"
#include "agent_information.hpp"
#include "i_event_topic_parser.hpp"

namespace smarthome
{

class IRegistrar;

class ControllerAgent : public IEventHandler, public IAgent, public std::tr1::enable_shared_from_this<ControllerAgent>
{
public:
    ControllerAgent(const AgentInformation& a_info, const SharedPtr<IEventTopicParser>& a_topicParser);
    virtual ~ControllerAgent() NOEXCEPTIONS; 

    virtual void Connect(const SharedPtr<IRegistrar>& a_registrar, const SharedPtr<IEventPusher>& a_pusher);
    virtual void Disconnect();

private:
    typedef IEventTopicParser::EventTopicsParsed EventTopicsSubscribed;

private:
    virtual void HandleEvent(const SharedPtr<Event>& a_event) NOEXCEPTIONS;

private:
    void subscribeToTopic(const SharedPtr<IRegistrar>& a_registrar, const EventTopic& a_topic);
    void subscribeToAll(const SharedPtr<IRegistrar>& a_registrar);

private:
    AgentInformation m_agentInfo;
    SharedPtr<EventTopicsSubscribed> m_eventsSubscribed;
    std::ofstream m_eventLogFile;
    bool m_isLogFileOpened;
    SharedPtr<IRegistrar> m_registrar;
};

} // namespace smarthome

#endif //CONTROLLER_AGENT_HPP