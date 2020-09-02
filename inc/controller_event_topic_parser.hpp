#ifndef CONTROLLER_EVENT_TOPIC_PARSER_HPP
#define CONTROLLER_EVENT_TOPIC_PARSER_HPP

#include <vector>
#include "common_utils.hpp"
#include "event_topic.hpp"
#include "i_event_topic_parser.hpp"

namespace smarthome
{

class AgentInformation;

class ControllerEventTopicParser : public IEventTopicParser
{
public:
    //EventTopicParser() = default;
    virtual ~ControllerEventTopicParser() NOEXCEPTIONS;

    virtual SharedPtr<EventTopicsParsed> Parse(const AgentInformation& a_agentInformation);
};

} //namespace smarthome

#endif //CONTROLLER_EVENT_TOPIC_PARSER_HPP