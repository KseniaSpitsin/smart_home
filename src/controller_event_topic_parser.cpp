#include <algorithm>
#include "controller_event_topic_parser.hpp"
#include "agent_information.hpp"
#include "configuration_file_details.hpp"

namespace smarthome
{

ControllerEventTopicParser::~ControllerEventTopicParser() NOEXCEPTIONS
{
}

SharedPtr<IEventTopicParser::EventTopicsParsed> ControllerEventTopicParser::Parse(const AgentInformation& a_agentInformation)
{
    SharedPtr<EventTopicsParsed> eventTopics(new EventTopicsParsed());
    const AgentConfig& agentConfig = a_agentInformation.GetConfig();

    if(0 == agentConfig.size())
    {
        return eventTopics;
    }

    AgentConfig::const_iterator startOfEventTopic = agentConfig.begin();
    AgentConfig::const_iterator eventTopicDelimiter;

    do
    {
        eventTopicDelimiter = std::find(startOfEventTopic, agentConfig.end(), EVENT_TOPIC_DELIMITER);
        AgentConfig::const_iterator eventTypeDelimiter = std::find(startOfEventTopic, agentConfig.end(), EVENT_TOPIC_INFORMATION_DELIMITER);
        EventType eventType(startOfEventTopic, eventTypeDelimiter);

        AgentConfig::const_iterator eventLocationFloorDelimiter = std::find(eventTypeDelimiter + 1, agentConfig.end(), EVENT_TOPIC_INFORMATION_DELIMITER);
        EventLocation::FloorType eventFloor(eventTypeDelimiter + 1, eventLocationFloorDelimiter);

        AgentConfig::const_iterator eventLocationRoomDelimiter = std::find(eventLocationFloorDelimiter + 1, agentConfig.end(), EVENT_TOPIC_DELIMITER);
        EventLocation::RoomType eventRoom(eventLocationFloorDelimiter + 1, eventLocationRoomDelimiter);

        eventTopics->push_back(EventTopic(eventType, EventLocation(eventFloor, eventRoom)));

        startOfEventTopic = eventTopicDelimiter + 1;
    } while (eventTopicDelimiter != agentConfig.end());

    return eventTopics;
}

} //namespace smarthome
