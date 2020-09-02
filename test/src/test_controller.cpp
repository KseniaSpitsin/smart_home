#include <algorithm>
#include "test_controller.hpp"
#include "i_registrar.hpp"
#include "event_topic.hpp"

namespace smarthome
{

class EventTopicComperator
{
public:
    EventTopicComperator(const EventTopic& a_eventTopic);
    bool operator()(const EventTopic& a_other);

private:
    EventTopic m_eventTopic;
};

EventTopicComperator::EventTopicComperator(const EventTopic& a_eventTopic)
    : m_eventTopic(a_eventTopic)
{
}

bool EventTopicComperator::operator()(const EventTopic& a_other)
{
    if(m_eventTopic.GetEventType() != a_other.GetEventType() && a_other.GetEventType() != ANY)
    {
        return false;
    }

    EventLocation thisEventLocation = m_eventTopic.GetEventLocation();
    EventLocation otherEventLocation = a_other.GetEventLocation();

    if(thisEventLocation == otherEventLocation)
    {
        return true;
    }

    if((otherEventLocation.GetRoom() == ANY && thisEventLocation.GetFloor() == otherEventLocation.GetRoom()) || otherEventLocation.GetRoom() == ANY)
    {
        return true;
    }
    
    return false;
}

TestController::TestController(const AgentInformation& a_info, const SharedPtr<IEventTopicParser> a_topicParser)
    : m_agentInfo(a_info)
    , m_eventsSubscribed(a_topicParser->Parse(a_info))
    , m_registrar()
    , m_countRightEvents(0)
{
}

TestController::~TestController() NOEXCEPTIONS
{
}

void TestController::Subscribe(const SharedPtr<IRegistrar>& a_registrar, const EventTopic& a_eventTopic)
{
    subscribeToTopic(a_registrar, a_eventTopic);
    m_eventsSubscribed->push_back(a_eventTopic);
}

void TestController::HandleEvent(const SharedPtr<Event>& a_event) NOEXCEPTIONS
{
    EventTopic eventTopic(a_event->GetType(), a_event->GetLocation());
    EventTopicsSubscribed::iterator eventFoundItr = std::find_if(m_eventsSubscribed->begin(), m_eventsSubscribed->end(), EventTopicComperator(eventTopic));
    if(m_eventsSubscribed->end() != eventFoundItr)
    {
        ++m_countRightEvents;
    }
}

size_t TestController::GetRightEventsNum() const NOEXCEPTIONS
{
    return m_countRightEvents;
}

void TestController::Connect(const SharedPtr<IRegistrar>& a_registrar, const SharedPtr<IEventPusher>& a_pusher)
{
    m_registrar = a_registrar;
    subscribeToAll(m_registrar);
}

void TestController::Disconnect()
{
    SharedPtr<TestController> self = shared_from_this();
    m_registrar->Unregister(self);
}

void TestController::subscribeToAll(const SharedPtr<IRegistrar>& a_registrar)
{
    size_t numOfEvents = m_eventsSubscribed->size();

    for(size_t i = 0; i < numOfEvents; ++i)
    {
        subscribeToTopic(a_registrar, m_eventsSubscribed->at(i));
    }
}

void TestController::subscribeToTopic(const SharedPtr<IRegistrar>& a_registrar, const EventTopic& a_topic)
{
    SharedPtr<TestController> self = shared_from_this();
    a_registrar->Registrate(self, a_topic);
}

} //namespace smarthome