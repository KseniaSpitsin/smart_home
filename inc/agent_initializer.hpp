#ifndef AGENT_INITIALIZER_HPP
#define AGENT_INITIALIZER_HPP

#include <string>
#include <vector>
#include "common_utils.hpp"
#include "agent_information.hpp"

namespace smarthome
{

class IAgent;
class IEventTopicParser;
class IEventTypeParser;

class AgentInitializer
{
public:
    typedef std::vector<SharedPtr<IAgent> > SystemAgents;

public:
    AgentInitializer();
    //~AgentInitializer() = default;
    //AgentInitializer(const AgentInitializer& a_other) = default;
    //AgentInitializer& operator=(const AgentInitializer& a_other) = default;

    //!@exception throws std::invalid_argument exception if can not open file or close file.
    SharedPtr<SystemAgents> InitializeFromFile(const std::string& a_configurationFileName, const SharedPtr<IEventTopicParser>& a_controllerParser,const SharedPtr<IEventTypeParser>& a_sensorParser);

private:
    static AgentInformation createAgentInformation(std::stringstream& a_agentInformationStream);

private:
    SharedPtr<IAgent> createAgent(const std::string& a_agentInformation, const SharedPtr<IEventTopicParser>& a_controllerParser, const SharedPtr<IEventTypeParser>& a_sensorParser);

private:
    advcpp::SoLibHandler m_libSensorHandler;
    advcpp::SoLibHandler m_libControllerHandler;
};

} //namespace smarthome

#endif //AGENT_INITIALIZER_HPP