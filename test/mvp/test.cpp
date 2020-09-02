#include "mu_test.h"
#include "test_utils.hpp"

#include "agent_information.hpp"
#include "event.hpp"
#include "event_middle_ware.hpp"
#include "event_distribution_list.hpp"
#include "test_controller.hpp"
#include "event_disseminator.hpp"
#include "hub_thread.hpp"
#include "location.hpp"
#include "sensor_event_type_parser.hpp"
#include "controller_event_topic_parser.hpp"
#include "thread_group.hpp"

using namespace smarthome;

UNIT(one_sensor_agent)
    AgentLocation loc("5", "7");
    AgentInformation info("light-1-a-1", "PhilipsHue", loc, "lights", "Hot");

    size_t eventMiddleWareCapacity = 5;

    SharedPtr<SensorEventTypeParser> sensorParser(new SensorEventTypeParser());
    SharedPtr<EventDistributionList> list(new EventDistributionList());
    SharedPtr<EventMiddleWare> ware = MakeMiddleWare(eventMiddleWareCapacity);

    SharedPtr<TestSensor> sa = MakeSensor(info, sensorParser, eventMiddleWareCapacity);
    sa->Connect(list, ware);
    sa->Disconnect();

    for(size_t i = 0; i < eventMiddleWareCapacity; ++i)
    {
        SharedPtr<Event> eventOut;
        ware->Pull(eventOut);

        ASSERT_EQUAL(eventOut->GetType(), info.GetConfig());
    }
END_UNIT

UNIT(one_sensor_one_controller)
    EventType temperatureType("Hot");
    EventLocation location("2", "5A");

    AgentInformation info("light-1-a-1", "PhilipsHue", location, "lights", "Hot");
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
    size_t eventNum = 1000;

    SharedPtr<EventMiddleWare> ware = MakeMiddleWare(eventMiddleWareCapacity);

    AgentInformation controllerInfo("light-1-a-1", "PhilipsHue", location, "lights", "Hot~2~5A");
    SharedPtr<TestController> controllerTemp(new TestController(controllerInfo, controllerParser));
    controllerTemp->Connect(list, ware);

    SharedPtr<TestSensor> sensorTemp = MakeSensor(info, sensorParser, eventNum);
    sensorTemp->Connect(list, ware);

    SharedPtr<HubThread> hub = MakeHubThread(dissaminator, ware, list, eventNum);

    hub->ShutDown();
    threadGroup->Join();

    sensorTemp->Disconnect();
    controllerTemp->Disconnect();

    ASSERT_EQUAL(controllerTemp->GetRightEventsNum(), eventNum);
END_UNIT

UNIT(two_sensor_agent_one_controller)
    EventType temperatureType("Hot");
    EventType fireType("Fire!");
    EventLocation location("2", "5A");

    AgentInformation infoSensorTemp("light-1-a-1", "PhilipsHue", location, "lights", "Hot");
    AgentInformation infoSensorFire("light-1-a-1", "PhilipsHue", location, "lights", "Fire!");
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
    size_t eventNum = 1000;

    SharedPtr<EventMiddleWare> ware = MakeMiddleWare(eventMiddleWareCapacity);

    EventTopic acvFloor2(temperatureType, Location("2"));

    AgentInformation infoAcvControllerFloor2("light-1-a-1", "PhilipsHue", location, "lights", "Hot~2~*");
    SharedPtr<TestController> acvControllerFloor2(new TestController(infoAcvControllerFloor2, controllerParser));
    acvControllerFloor2->Connect(list, ware);

    SharedPtr<HubThread> hub = MakeHubThread(dissaminator, ware, list, eventNum);

    SharedPtr<TestSensor> sensorTemp = MakeSensor(infoSensorTemp, sensorParser, eventNum/2);
    sensorTemp->Connect(list, ware);

    SharedPtr<TestSensor> sensorFire = MakeSensor(infoSensorFire, sensorParser, eventNum/2);
    sensorFire->Connect(list, ware);

    sensorTemp->Disconnect();
    sensorFire->Disconnect();

    hub->ShutDown();
    threadGroup->Join();

    acvControllerFloor2->Disconnect();

    ASSERT_EQUAL(acvControllerFloor2->GetRightEventsNum(), eventNum/2);
END_UNIT

UNIT(two_sensor_agent_two_controllers)
    EventType fireType("Fire!");
    EventLocation floor2Room45("2", "45");
    EventLocation floor2Room4("2", "4");

    AgentInformation infoAgentFloor2Room45("light-1-a-1", "PhilipsHue", floor2Room45, "lights", "Fire!");
    AgentInformation infoAgentFloor2Room4("light-1-a-1", "PhilipsHue", floor2Room4, "lights", "Fire!");

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
    size_t eventNum = 100;

    SharedPtr<EventMiddleWare> ware = MakeMiddleWare(eventMiddleWareCapacity);

    AgentInformation controllerOfSprinklersInfo("light-1-a-1", "PhilipsHue", floor2Room45, "lights", "Fire!~2~45");
    SharedPtr<TestController> controllerOfSprinklers(new TestController(controllerOfSprinklersInfo, controllerParser));
    controllerOfSprinklers->Connect(list, ware);

    AgentInformation controllerOfACVInfo("light-1-a-1", "PhilipsHue", floor2Room45, "lights", "Fire!~2~*|Fire!~3~*");
    SharedPtr<TestController> controllerOfACV(new TestController(controllerOfACVInfo, controllerParser));
    controllerOfACV->Connect(list, ware);

    SharedPtr<HubThread> hub = MakeHubThread(dissaminator, ware, list, eventNum);

    SharedPtr<TestSensor> sensorFireFloor2Room45 = MakeSensor(infoAgentFloor2Room45, sensorParser, eventNum/2);
    sensorFireFloor2Room45->Connect(list, ware);

    SharedPtr<TestSensor> sensorFireFloor2Room4 = MakeSensor(infoAgentFloor2Room4, sensorParser, eventNum/2);
    sensorFireFloor2Room4->Connect(list, ware);

    sensorFireFloor2Room4->Disconnect();
    sensorFireFloor2Room45->Disconnect();

    hub->ShutDown();
    threadGroup->Join();

    controllerOfACV->Disconnect();
    controllerOfSprinklers->Disconnect();

    ASSERT_EQUAL(controllerOfSprinklers->GetRightEventsNum(), eventNum/2);
    ASSERT_EQUAL(controllerOfACV->GetRightEventsNum(), eventNum);
END_UNIT

UNIT(test_3_sensors_5_controllers)
    EventType smoke("SMOKE");
    EventType temp("TEMP");
    EventType movement("MOVE_DETECTED");
    EventLocation floor2("2");
    EventLocation floor2Room2A("2", "2A");
    EventLocation floor5Room7D("5", "7D");

    AgentInformation infoSensorTempAgentFloor2Room2A("light-1-a-1", "PhilipsHue", floor2Room2A, "lights", "TEMP");
    AgentInformation infoSensorSmokeAgentFloor2Room2A("light-1-a-1", "PhilipsHue", floor2Room2A, "lights", "SMOKE");
    AgentInformation infoSensorSmokeAgentFloor5Room7D("light-1-a-1", "PhilipsHue", floor5Room7D, "lights", "SMOKE");

    SharedPtr<EventDistributionList> list(new EventDistributionList());
    
    SharedPtr<ChannelsContainer> channels = MakeChannels();
    SharedPtr<EventPackagePushers> packagePushers = MakePackagePushers(channels);
    SharedPtr<EventDisseminator> dissaminator(new EventDisseminator(*packagePushers));

    SharedPtr<SensorEventTypeParser> sensorParser(new SensorEventTypeParser());
    SharedPtr<ControllerEventTopicParser> controllerParser(new ControllerEventTopicParser());

    size_t eventMiddleWareCapacity = 13;
    size_t eventNum = 101;
    size_t numOfSensors = 3;
    ThreadGroup::TasksContainer tasks;
    size_t numOfChannels = channels->size();
    tasks.reserve(numOfChannels);
    for(size_t i = 0; i < numOfChannels; ++i)
    {
        SharedPtr<EventEmissary> emissaryTask(new EventEmissary((*channels)[i]));
        tasks.push_back(emissaryTask);
    }
    SharedPtr<ThreadGroup> threadGroup(new ThreadGroup(tasks));

    SharedPtr<EventMiddleWare> ware = MakeMiddleWare(eventMiddleWareCapacity);

    AgentInformation sprinklersControllerFloor2Info("light-1-a-1", "PhilipsHue", floor2Room2A, "lights", "SMOKE~2~*|TEMP~2~*");
    SharedPtr<TestController> sprinklersControllerFloor2(new TestController(sprinklersControllerFloor2Info, controllerParser));
    sprinklersControllerFloor2->Connect(list, ware);

    AgentInformation acvControllerFloor2Room2AInfo("light-1-a-1", "PhilipsHue", floor2Room2A, "lights", "MOVE_DETECTED~2~2A|TEMP~2~2A");
    SharedPtr<TestController> acvControllerFloor2Room2A(new TestController(acvControllerFloor2Room2AInfo, controllerParser));
    acvControllerFloor2Room2A->Connect(list, ware);

    AgentInformation sprinklersControllerFloor2Room2AInfo("light-1-a-1", "PhilipsHue", floor2Room2A, "lights", "TEMP~2~2A|MOVE_DETECTED~2~2A");
    SharedPtr<TestController> sprinklersControllerFloor2Room2(new TestController(sprinklersControllerFloor2Room2AInfo, controllerParser));
    sprinklersControllerFloor2Room2->Connect(list, ware);

    AgentInformation acvControllerFloor5Room7DInfo("light-1-a-1", "PhilipsHue", floor5Room7D, "lights", "TEMP~5~7D|SMOKE~5~7D");
    SharedPtr<TestController> acvControllerFloor5Room7D(new TestController(acvControllerFloor5Room7DInfo, controllerParser));
    acvControllerFloor5Room7D->Connect(list, ware);

    AgentInformation generalSprinklersControllerInfo("light-1-a-1", "PhilipsHue", floor5Room7D, "lights", "SMOKE~*~*|TEMP~*~*");
    SharedPtr<TestController> generalSprinklersController(new TestController(generalSprinklersControllerInfo, controllerParser));
    generalSprinklersController->Connect(list, ware);

    SharedPtr<HubThread> hub = MakeHubThread(dissaminator, ware, list, eventNum * numOfSensors);

    SharedPtr<TestSensor> sensorTempFloor2Room2A = MakeSensor(infoSensorTempAgentFloor2Room2A, sensorParser, eventNum);
    sensorTempFloor2Room2A->Connect(list, ware);

    SharedPtr<TestSensor> sensorSmokeFloor2Room2A = MakeSensor(infoSensorSmokeAgentFloor2Room2A, sensorParser, eventNum);
    sensorSmokeFloor2Room2A->Connect(list, ware);

    SharedPtr<TestSensor> sensorSmokeFloor5Room7D = MakeSensor(infoSensorSmokeAgentFloor5Room7D, sensorParser, eventNum);
    sensorSmokeFloor5Room7D->Connect(list, ware);

    sensorTempFloor2Room2A->Disconnect();
    sensorSmokeFloor2Room2A->Disconnect();
    sensorSmokeFloor5Room7D->Disconnect();

    hub->ShutDown();
    threadGroup->Join();
    
    sprinklersControllerFloor2->Disconnect();
    acvControllerFloor2Room2A->Disconnect();
    sprinklersControllerFloor2Room2->Disconnect();
    acvControllerFloor5Room7D->Disconnect();
    generalSprinklersController->Disconnect();

    ASSERT_EQUAL(sprinklersControllerFloor2->GetRightEventsNum(), 202);
    ASSERT_EQUAL(acvControllerFloor2Room2A->GetRightEventsNum(), 101);
    ASSERT_EQUAL(sprinklersControllerFloor2Room2->GetRightEventsNum(), 101);
    ASSERT_EQUAL(acvControllerFloor5Room7D->GetRightEventsNum(), 101);
    ASSERT_EQUAL(generalSprinklersController->GetRightEventsNum(), 303);
END_UNIT

TEST_SUITE(mvp)
    TEST(one_sensor_agent)
    TEST(one_sensor_one_controller)
    TEST(two_sensor_agent_one_controller)
    TEST(two_sensor_agent_two_controllers)
    TEST(test_3_sensors_5_controllers)
END_SUITE