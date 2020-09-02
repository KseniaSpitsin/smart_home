#include <string>
#include <unistd.h>

#include "mu_test.h"
#include "test_utils.hpp"

#include "agent_initializer.hpp"
#include "event_middle_ware.hpp"
#include "event_distribution_list.hpp"
#include "sensor_event_type_parser.hpp"
#include "controller_event_topic_parser.hpp"
#include "event_disseminator.hpp"
#include "thread_group.hpp"
#include "hub.hpp"
#include "i_agent.hpp"

using namespace smarthome;

const std::string CONFIGURATION_FILE = "./configuration.txt";

UNIT(initialize_system)
    SharedPtr<AgentInitializer> initializer(new AgentInitializer());
    SharedPtr<EventDistributionList> list(new EventDistributionList());
    SharedPtr<ChannelsContainer> channels = MakeChannels();
    SharedPtr<EventPackagePushers> packagePushers = MakePackagePushers(channels);
    SharedPtr<EventDisseminator> dissaminator(new EventDisseminator(*packagePushers));

    SharedPtr<SensorEventTypeParser> sensorParser(new SensorEventTypeParser());
    SharedPtr<ControllerEventTopicParser> controllerParser(new ControllerEventTopicParser());

    ThreadGroup::TasksContainer tasks;
    size_t numOfChannels = channels->size();
    tasks.reserve(numOfChannels);
    for(size_t i = 0; i < numOfChannels; ++i)
    {
        SharedPtr<EventEmissary> emissaryTask(new EventEmissary((*channels)[i]));
        tasks.push_back(emissaryTask);
    }
    SharedPtr<ThreadGroup> threadGroup(new ThreadGroup(tasks));

    size_t eventMiddleWareCapacity = 10;

    SharedPtr<AgentInitializer::SystemAgents> agents = initializer->InitializeFromFile(CONFIGURATION_FILE, controllerParser, sensorParser);

    SharedPtr<EventMiddleWare> ware = MakeMiddleWare(eventMiddleWareCapacity);

    size_t numOfAgents = agents->size();

    for(size_t i = 0; i < numOfAgents; ++i)
    {
        agents->at(i)->Connect(list, ware);
    }

    SharedPtr<Hub> hub(new Hub(dissaminator, ware, list));

    usleep(10000);

    ware->ShutDown();
    hub->ShutDown();
    threadGroup->Join();

    for(size_t i = 0; i < numOfAgents; ++i)
    {
        agents->at(i)->Disconnect();
    }

    ASSERT_THAT(true);
END_UNIT

TEST_SUITE(so)
    TEST(initialize_system)
END_SUITE