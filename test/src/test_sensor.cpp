#include "test_sensor.hpp"
#include "event.hpp"
#include "i_event_pusher.hpp"
#include "event_pusher_shut_down_exception.hpp"
#include "i_event_type_parser.hpp"

namespace smarthome
{

TestSensor::TestSensor(const AgentInformation& a_info, const SharedPtr<IEventTypeParser>& a_parser, size_t a_numOfEvents)
    : Runnable()
    , m_info(a_info)
    , m_numOfEvents(a_numOfEvents)
    , m_parser(a_parser)
    , m_pusher()
    , m_thread()
{
}

TestSensor::~TestSensor() NOEXCEPTIONS
{
}

void TestSensor::Connect(const SharedPtr<IRegistrar>& a_registrar, const SharedPtr<IEventPusher>& a_pusher)
{
    m_pusher = a_pusher;
    m_thread = SharedPtr<advcpp::RThread>(new advcpp::RThread(*this));
}

void TestSensor::Disconnect()
{
    m_thread->Join();
}

void TestSensor::Run() NOEXCEPTIONS
{

    EventType type = m_parser->Parse(m_info);
    const EventLocation& location =  m_info.GetLocation();

    for(size_t i = 0; i < m_numOfEvents; ++i)
    {
        try
        {
            SharedPtr<Event> event(new Event(type, location, "now"));
            m_pusher->Push(event);
        }
        catch(const EventPusherShutDownException&)
        {
            break;
        }
        catch(const std::exception&)
        {
            continue;
        }
    }
}

} //namespace smarthome