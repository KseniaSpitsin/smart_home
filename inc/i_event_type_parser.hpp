#ifndef I_EVENT_TYPE_PARSER_HPP
#define I_EVENT_TYPE_PARSER_HPP

#include "common_utils.hpp"
#include "event.hpp"

namespace smarthome
{

class AgentInformation;

class IEventTypeParser : private advcpp::Uncopyable
{
public:
    //IEventTopicParser() = default;
    virtual ~IEventTypeParser();

    virtual EventType Parse(const AgentInformation& a_agentInformation) = 0;
};

} //namespace smarthome

#endif //I_EVENT_TYPE_PARSER_HPP