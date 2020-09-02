#include <algorithm>
#include "controller_agent.hpp"
#include "i_registrar.hpp"
#include "event_topic.hpp"

namespace smarthome
{

extern "C" SharedPtr<IAgent> MakeControllerAgent(const AgentInformation& a_info, const SharedPtr<IEventTopicParser>& a_topicParser)
{
    return SharedPtr<IAgent>(new ControllerAgent(a_info, a_topicParser));
}


ControllerAgent::ControllerAgent(const AgentInformation& a_info, const SharedPtr<IEventTopicParser>& a_topicParser)
    : m_agentInfo(a_info)
    , m_eventsSubscribed(a_topicParser->Parse(a_info))
    , m_eventLogFile()
    , m_isLogFileOpened(false)
    , m_registrar()
{
    const AgentLog& lofFile = m_agentInfo.GetLog();
    m_eventLogFile.open(lofFile.c_str());

    if(m_eventLogFile.is_open())
    {
        m_isLogFileOpened = true;
    }
}

ControllerAgent::~ControllerAgent() NOEXCEPTIONS
{
    if(m_isLogFileOpened)
    {
        m_eventLogFile.close();
    }
}

void ControllerAgent::HandleEvent(const SharedPtr<Event>& a_event) NOEXCEPTIONS
{
    if(m_isLogFileOpened)
    {
        m_eventLogFile << "Controller " << m_agentInfo.GetId() << " received event " << a_event->GetType();
        EventLocation location = a_event->GetLocation();
        m_eventLogFile << ":" << location.GetFloor() << ":" << location.GetRoom() << std::endl;
    }
}

void ControllerAgent::Connect(const SharedPtr<IRegistrar>& a_registrar, const SharedPtr<IEventPusher>& a_pusher)
{
    m_registrar = a_registrar;
    subscribeToAll(m_registrar);
}

void ControllerAgent::Disconnect()
{
    SharedPtr<ControllerAgent> self = shared_from_this();
    m_registrar->Unregister(self);
}

void ControllerAgent::subscribeToAll(const SharedPtr<IRegistrar>& a_registrar)
{
    size_t numOfEvents = m_eventsSubscribed->size();

    for(size_t i = 0; i < numOfEvents; ++i)
    {
        subscribeToTopic(a_registrar, m_eventsSubscribed->at(i));
    }
}

void ControllerAgent::subscribeToTopic(const SharedPtr<IRegistrar>& a_registrar, const EventTopic& a_topic)
{
    SharedPtr<ControllerAgent> self = shared_from_this();
    a_registrar->Registrate(self, a_topic);
}

} //namespace smarthome