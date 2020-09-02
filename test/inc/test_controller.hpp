#ifndef TEST_CONTROLLER_HPP
#define TEST_CONTROLLER_HPP

#include <vector>

#include "i_event_handler.hpp"
#include "i_agent.hpp"
#include "agent_information.hpp"
#include "event_topic.hpp"
#include "i_event_topic_parser.hpp"

namespace smarthome
{

class IRegistrar;

class TestController : public IEventHandler, public IAgent, public std::tr1::enable_shared_from_this<TestController>
{
public:
    TestController(const AgentInformation& a_info, const SharedPtr<IEventTopicParser> a_topicParser);
    virtual ~TestController() NOEXCEPTIONS; 

    virtual void Connect(const SharedPtr<IRegistrar>& a_registrar, const SharedPtr<IEventPusher>& a_pusher);
    virtual void Disconnect();
    
    virtual void Subscribe(const SharedPtr<IRegistrar>& a_registrar, const EventTopic& a_eventTopic);

    size_t GetRightEventsNum() const NOEXCEPTIONS;

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
    SharedPtr<IRegistrar> m_registrar;

    size_t m_countRightEvents;
};

} // namespace smarthome

#endif //TEST_CONTROLLER_HPP