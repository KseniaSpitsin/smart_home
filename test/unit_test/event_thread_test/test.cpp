#include "mu_test.h"

#include <vector>
#include <algorithm>

#include "common_utils.hpp"
#include "distribution_info.hpp"
#include "event_emissary.hpp"
#include "event_channel.hpp"
#include "event_channel_configuration.hpp"
#include "event_middle_ware.hpp"
#include "event_distribution_list.hpp"
#include "event_disseminator.hpp"
#include "hub.hpp"
#include "test_utils.hpp"
#include "test_controller.hpp"
#include "controller_event_topic_parser.hpp"
#include "thread_group.hpp"

using namespace smarthome;

UNIT(create_system_with_5_controllers)
    AgentLocation loc("room_1_a", "1");
    AgentInformation info("light-1-a-1", "PhilipsHue", loc, "lights");
    SharedPtr<EventDistributionList> list(new EventDistributionList());
    
    SharedPtr<ChannelsContainer> channels = MakeChannels();
    SharedPtr<EventPackagePushers> packagePushers = MakePackagePushers(channels);
    SharedPtr<EventDisseminator> dissaminator(new EventDisseminator(*packagePushers));

    size_t numOfEventsOfEachType = 100;
    size_t numOfEventTypes = 4;
    size_t eventMiddleWareCapacity = 30;

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

    EventType smoke = "SMOKE";
    EventType temp = "TEMP";
    EventType movement = "MOVE_DETECTED";
    EventLocation floor2("2");
    EventLocation floor2Room2A("2", "2A");
    EventLocation floor5Room7D("5", "7D");

    SharedPtr<ControllerEventTopicParser> dummyControllerParser(new ControllerEventTopicParser());

    SharedPtr<TestController> sprinklersControllerFloor2(new TestController(info, dummyControllerParser));
    sprinklersControllerFloor2->Subscribe(list, EventTopic(smoke, floor2));
    sprinklersControllerFloor2->Subscribe(list, EventTopic(temp, floor2));

    SharedPtr<TestController> acvControllerFloor2Room2A(new TestController(info, dummyControllerParser));
    acvControllerFloor2Room2A->Subscribe(list, EventTopic(movement, floor2Room2A));
    acvControllerFloor2Room2A->Subscribe(list, EventTopic(temp, floor2Room2A));

    SharedPtr<TestController> sprinklersControllerFloor2Room2A(new TestController(info, dummyControllerParser));
    sprinklersControllerFloor2Room2A->Subscribe(list, EventTopic(temp, floor2Room2A));
    sprinklersControllerFloor2Room2A->Subscribe(list, EventTopic(movement, floor2Room2A));

    SharedPtr<TestController> acvControllerFloor5Room7D(new TestController(info, dummyControllerParser));
    acvControllerFloor5Room7D->Subscribe(list, EventTopic(temp, floor5Room7D));
    acvControllerFloor5Room7D->Subscribe(list, EventTopic(smoke, floor5Room7D));

    SharedPtr<TestController> generalSprinklersController(new TestController(info, dummyControllerParser));
    generalSprinklersController->Subscribe(list, smoke);
    generalSprinklersController->Subscribe(list, temp);

    SharedPtr<Event> testedEventTempFloor2Room2A(new Event(temp, floor2Room2A, "7:30"));
    SharedPtr<Event> testedEventSmokeFloor2Room2A(new Event(smoke, floor2Room2A, "2:51"));
    SharedPtr<Event> testedEventSmokeFloor5Room7D(new Event(smoke, floor5Room7D, "2:51"));
    SharedPtr<Event> testedEventMovementFloor2Room2A(new Event(movement, floor2Room2A, "2:20"));
    
    SharedPtr<HubThread> hub = MakeHubThread(dissaminator, ware, list, numOfEventsOfEachType * numOfEventTypes);

    for(size_t i = 0; i < numOfEventsOfEachType; ++i)
    {
        ware->Push(testedEventTempFloor2Room2A);
        ware->Push(testedEventSmokeFloor2Room2A);
        ware->Push(testedEventSmokeFloor5Room7D);
        ware->Push(testedEventMovementFloor2Room2A);
    }

    hub->ShutDown();
    threadGroup->Join();
    
    ASSERT_EQUAL(sprinklersControllerFloor2->GetRightEventsNum(), 200);
    ASSERT_EQUAL(acvControllerFloor2Room2A->GetRightEventsNum(), 200);
    ASSERT_EQUAL(sprinklersControllerFloor2Room2A->GetRightEventsNum(), 200);
    ASSERT_EQUAL(acvControllerFloor5Room7D->GetRightEventsNum(), 100);
    ASSERT_EQUAL(generalSprinklersController->GetRightEventsNum(), 300);
END_UNIT

TEST_SUITE(event_thread)
    TEST(create_system_with_5_controllers)
END_SUITE