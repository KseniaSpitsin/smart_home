#include "sensor_agent.hpp"
#include "event.hpp"
#include "i_event_pusher.hpp"
#include "event_pusher_shut_down_exception.hpp"
#include "i_event_type_parser.hpp"

namespace smarthome
{

extern "C"
{

SharedPtr<IAgent> MakeSensorAgent(const AgentInformation& a_info, const SharedPtr<IEventTypeParser>& a_parser)
{
    return SharedPtr<IAgent>(new SensorAgent(a_info, a_parser));
}

} //extern "C"

SensorAgent::SensorAgent(const AgentInformation& a_info, const SharedPtr<IEventTypeParser>& a_parser)
    : Runnable()
    , m_info(a_info)
    , m_parser(a_parser)
    , m_thread()
    , m_isRunning(true)
    , m_pusher()
{
}

SensorAgent::~SensorAgent() NOEXCEPTIONS
{
}

void SensorAgent::Connect(const SharedPtr<IRegistrar>& a_registrar, const SharedPtr<IEventPusher>& a_pusher)
{
    m_pusher = a_pusher;
    m_thread = SharedPtr<advcpp::RThread>(new advcpp::RThread(*this));
}

void SensorAgent::Disconnect()
{
    if(m_isRunning.Get())
    {
        m_isRunning.Release();
    }
    m_thread->Join();
}

void SensorAgent::Run() NOEXCEPTIONS
{

    EventType type = m_parser->Parse(m_info);
    const EventLocation& location =  m_info.GetLocation();

    while(m_isRunning.Get())
    {
        try
        {
            SharedPtr<Event> event(new Event(type, location, "now"));
            m_pusher->Push(event);
        }
        catch(const EventPusherShutDownException&)
        {
            m_isRunning.Release();
        }
        catch(const std::exception&)
        {
            continue;
        }
    }
}

} //namespace smarthome