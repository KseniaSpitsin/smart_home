#include "sensor_event_type_parser.hpp"
#include "agent_information.hpp"

namespace smarthome
{

SensorEventTypeParser::~SensorEventTypeParser() NOEXCEPTIONS
{
}

EventType SensorEventTypeParser::Parse(const AgentInformation& a_agentInformation)
{
    return EventType(a_agentInformation.GetConfig());
}

} //namespace smarthome