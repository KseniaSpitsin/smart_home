#include <fstream>
#include <sstream>
#include <iostream>
#include "agent_initializer.hpp"
#include "configuration_file_details.hpp"

namespace smarthome
{

typedef SharedPtr<IAgent> (*CreateController)(const AgentInformation& a_info, const SharedPtr<IEventTopicParser>& a_topicParser);
typedef SharedPtr<IAgent> (*CreateSensor)(const AgentInformation& a_info, const SharedPtr<IEventTypeParser>& a_parser);

AgentInitializer::AgentInitializer()
    : m_libSensorHandler("./libSensorAgent.so")
    , m_libControllerHandler("./libControllerAgent.so")
{
}

SharedPtr<AgentInitializer::SystemAgents> AgentInitializer::InitializeFromFile(const std::string& a_configurationFileName, const SharedPtr<IEventTopicParser>& a_controllerParser,const SharedPtr<IEventTypeParser>& a_sensorParser)
{
    std::ifstream configurationFile;
    configurationFile.open(a_configurationFileName.c_str());

    if(!configurationFile.is_open())
    {
        throw std::invalid_argument("Could not open file");
    }

    SharedPtr<SystemAgents> agents(new SystemAgents());
    std::string line;
    while(getline(configurationFile, line))
    {
        agents->push_back(createAgent(line, a_controllerParser, a_sensorParser));
    }

    return agents;
}

SharedPtr<IAgent> AgentInitializer::createAgent(const std::string& a_agentInformation, const SharedPtr<IEventTopicParser>& a_controllerParser, const SharedPtr<IEventTypeParser>& a_sensorParser)
{
    std::stringstream information(a_agentInformation);

    std::string agentType;
    getline(information, agentType, AGENT_INFORMATION_DETAILS_DELIMITER);
    
    AgentInformation agentInformation = createAgentInformation(information);
    SharedPtr<IAgent> agentCreated;

    if(CONTROLLER == agentType)
    {
        CreateController controllerMaker = m_libControllerHandler.getFunction<CreateController>("MakeControllerAgent");
        agentCreated = controllerMaker(agentInformation, a_controllerParser);
    }
    else
    {
        CreateSensor sensorMaker = m_libSensorHandler.getFunction<CreateSensor>("MakeSensorAgent");
        agentCreated = sensorMaker(agentInformation, a_sensorParser);
    }

    return agentCreated;
}

AgentInformation AgentInitializer::createAgentInformation(std::stringstream& a_agentInformationStream)
{
    AgentId agentId;
    getline(a_agentInformationStream, agentId, AGENT_INFORMATION_DETAILS_DELIMITER);

    AgentDevice agentDevice;
    getline(a_agentInformationStream, agentDevice, AGENT_INFORMATION_DETAILS_DELIMITER);

    AgentLocation::RoomType agentRoom;
    getline(a_agentInformationStream, agentRoom, AGENT_INFORMATION_DETAILS_DELIMITER);

    AgentLocation::FloorType agentFloor;
    getline(a_agentInformationStream, agentFloor, AGENT_INFORMATION_DETAILS_DELIMITER);

    AgentLog agentLog;
    getline(a_agentInformationStream, agentLog, AGENT_INFORMATION_DETAILS_DELIMITER);

    AgentConfig agentConfig;
    getline(a_agentInformationStream, agentConfig, AGENT_INFORMATION_DETAILS_DELIMITER);

    return AgentInformation(agentId, agentDevice, AgentLocation(agentRoom, agentFloor), agentLog, agentConfig);
}

} //namespace smarthome