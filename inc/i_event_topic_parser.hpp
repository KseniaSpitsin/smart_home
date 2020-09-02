#ifndef I_EVENT_TOPIC_PARSER_HPP
#define I_EVENT_TOPIC_PARSER_HPP

#include <vector>
#include "common_utils.hpp"
#include "event_topic.hpp"

namespace smarthome
{

class AgentInformation;

class IEventTopicParser : private advcpp::Uncopyable
{
public:
    typedef std::vector<EventTopic> EventTopicsParsed;

public:
    //IEventTopicParser() = default;
    virtual ~IEventTopicParser() NOEXCEPTIONS;

    virtual SharedPtr<EventTopicsParsed> Parse(const AgentInformation& a_agentInformation) = 0;
};

} //namespace smarthome

#endif //I_EVENT_TOPIC_PARSER_HPP