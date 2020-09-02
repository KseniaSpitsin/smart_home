#ifndef SENSOR_EVENT_TYPE_PARSER_HPP
#define SENSOR_EVENT_TYPE_PARSER_HPP

#include "common_utils.hpp"
#include "i_event_type_parser.hpp"

namespace smarthome
{

class AgentInformation;

class SensorEventTypeParser : public IEventTypeParser
{
public:
    //SensorEventTypeParser() = default;
    virtual ~SensorEventTypeParser() NOEXCEPTIONS;

    virtual EventType Parse(const AgentInformation& a_agentInformation);
};

} //namespace smarthome

#endif //SENSOR_EVENT_TYPE_PARSER_HPP