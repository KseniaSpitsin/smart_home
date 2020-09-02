#include "mu_test.h"
#include "event_distribution_list.hpp"
#include "test_controller.hpp"
#include "agent_information.hpp"
#include "test_utils.hpp"
#include "subscribers_list.hpp"
#include "subscribers_list_itr.hpp"
#include "distribution_info.hpp"
#include "i_event_handler.hpp"
#include "event_topic.hpp"
#include "controller_event_topic_parser.hpp"

using namespace smarthome;

void SubscribersListHandleEvent(SubscribersList& a_subList, const SharedPtr<Event>& a_event)
{
    SubscribersListItr first = a_subList.begin();
    SubscribersListItr last = a_subList.end();

    while(first != last)
    {
        SharedPtr<IEventHandler> handler = first->GetEventHandler();
        handler->HandleEvent(a_event);
        ++first;
    }
}

UNIT(no_overlap_no_wild_cards)
    AgentLocation loc("room_1_a", "1");
    AgentInformation info("light-1-a-1", "PhilipsHue", loc, "lights", "iot:1003,sat:242, bright:154, hue:8200");
    SharedPtr<EventDistributionList> list(new EventDistributionList());

    EventType smoke = "SMOKE";
    EventType temp = "TEMP";
    EventLocation floor1Room7B("1", "7B");
    EventLocation floor1Room2A("1", "2A");
    EventLocation floor1Room5A("1", "5A");
    EventLocation floor5Room7D("5", "7D");
    EventLocation floor2Room1A("2", "1A");

    SharedPtr<ControllerEventTopicParser> dummyControllerParser(new ControllerEventTopicParser());

    SharedPtr<TestController> sprinklersControllerFloor1Room7B(new TestController(info, dummyControllerParser));
    sprinklersControllerFloor1Room7B->Subscribe(list, EventTopic(smoke, floor1Room7B));
    sprinklersControllerFloor1Room7B->Subscribe(list, EventTopic(temp, floor1Room7B));

    SharedPtr<TestController> sprinklersControllerFloor1Room2A(new TestController(info, dummyControllerParser));
    sprinklersControllerFloor1Room2A->Subscribe(list, EventTopic(smoke, floor1Room2A));

    SharedPtr<TestController> sprinklersControllerFloor1Room5A(new TestController(info, dummyControllerParser));
    sprinklersControllerFloor1Room5A->Subscribe(list, EventTopic(temp, floor1Room5A));

    SharedPtr<TestController> acvControllerFloor5Room7D(new TestController(info, dummyControllerParser));
    acvControllerFloor5Room7D->Subscribe(list, EventTopic(temp, floor5Room7D));
    acvControllerFloor5Room7D->Subscribe(list, EventTopic(smoke, floor5Room7D));

    SharedPtr<TestController> acvControllerFloor2Room1A(new TestController(info, dummyControllerParser));
    acvControllerFloor2Room1A->Subscribe(list, EventTopic(temp, floor2Room1A));
    acvControllerFloor2Room1A->Subscribe(list, EventTopic(smoke, floor2Room1A));

    SharedPtr<Event> testedEvent(new Event(temp, floor1Room5A, "7:30"));

    SubscribersList returnedList = list->SearchEventSubscribers(EventTopic(testedEvent->GetType(), testedEvent->GetLocation()));

    SubscribersListHandleEvent(returnedList, testedEvent);

    ASSERT_EQUAL(sprinklersControllerFloor1Room7B->GetRightEventsNum(), 0);
    ASSERT_EQUAL(sprinklersControllerFloor1Room2A->GetRightEventsNum(), 0);
    ASSERT_EQUAL(sprinklersControllerFloor1Room5A->GetRightEventsNum(), 1);
    ASSERT_EQUAL(acvControllerFloor5Room7D->GetRightEventsNum(), 0);
    ASSERT_EQUAL(acvControllerFloor2Room1A->GetRightEventsNum(), 0);
END_UNIT


UNIT(wild_cards_no_overlap)
    AgentLocation loc("room_1_a", "1");
    AgentInformation info("light-1-a-1", "PhilipsHue", loc, "lights", "iot:1003,sat:242, bright:154, hue:8200");
    SharedPtr<EventDistributionList> list(new EventDistributionList());

    EventType smoke = "SMOKE";
    EventType temp = "TEMP";
    EventType movement = "MOVE_DETECTED";
    EventLocation floor1AnyRoom("1");
    EventLocation floor2Room2A("2", "2A");
    EventLocation floor2Room5A("2", "5A");
    EventLocation floor5Room7D("5", "7D");

    SharedPtr<ControllerEventTopicParser> dummyControllerParser(new ControllerEventTopicParser());

    SharedPtr<TestController> sprinklersControllerFloor1(new TestController(info, dummyControllerParser));
    sprinklersControllerFloor1->Subscribe(list, EventTopic(smoke, floor1AnyRoom));
    sprinklersControllerFloor1->Subscribe(list, EventTopic(temp, floor1AnyRoom));

    SharedPtr<TestController> sprinklersControllerFloor2Room2A(new TestController(info, dummyControllerParser));
    sprinklersControllerFloor2Room2A->Subscribe(list, EventTopic(smoke, floor2Room2A));

    SharedPtr<TestController> sprinklersControllerFloor2Room5A(new TestController(info, dummyControllerParser));
    sprinklersControllerFloor2Room5A->Subscribe(list, EventTopic(temp, floor2Room5A));

    SharedPtr<TestController> acvControllerFloor5Room7D(new TestController(info, dummyControllerParser));
    acvControllerFloor5Room7D->Subscribe(list, EventTopic(temp, floor5Room7D));
    acvControllerFloor5Room7D->Subscribe(list, EventTopic(smoke, floor5Room7D));

    SharedPtr<TestController> alarmController(new TestController(info, dummyControllerParser));
    alarmController->Subscribe(list, movement);

    SharedPtr<Event> testedEventTemp(new Event(temp, Location("1", "5A"), "7:30"));
    SharedPtr<Event> testedEventMove(new Event(movement, floor5Room7D, "2:51"));

    SubscribersList subscribedToTestedEventTemp = list->SearchEventSubscribers(EventTopic(testedEventTemp->GetType(), testedEventTemp->GetLocation()));
    SubscribersList subscribedToTestedEventMove = list->SearchEventSubscribers(EventTopic(testedEventMove->GetType(), testedEventMove->GetLocation()));

    SubscribersListHandleEvent(subscribedToTestedEventTemp, testedEventTemp);

    SubscribersListHandleEvent(subscribedToTestedEventMove, testedEventMove);

    ASSERT_EQUAL(sprinklersControllerFloor1->GetRightEventsNum(), 1);
    ASSERT_EQUAL(sprinklersControllerFloor2Room2A->GetRightEventsNum(), 0);
    ASSERT_EQUAL(sprinklersControllerFloor2Room5A->GetRightEventsNum(), 0);
    ASSERT_EQUAL(acvControllerFloor5Room7D->GetRightEventsNum(), 0);
    ASSERT_EQUAL(alarmController->GetRightEventsNum(), 1);
END_UNIT

UNIT(wild_cards_with_overlap_room)
    AgentLocation loc("room_1_a", "1");
    AgentInformation info("light-1-a-1", "PhilipsHue", loc, "lights", "iot:1003,sat:242, bright:154, hue:8200");
    SharedPtr<EventDistributionList> list(new EventDistributionList());

    EventType smoke = "SMOKE";
    EventType temp = "TEMP";
    EventType movement = "MOVE_DETECTED";
    EventLocation floor2Room2A("2", "2A");
    EventLocation floor2Room5A("2", "5A");
    EventLocation floor5Room7D("5", "7D");

    SharedPtr<ControllerEventTopicParser> dummyControllerParser(new ControllerEventTopicParser());

    SharedPtr<TestController> sprinklersControllerFloor2Room2A(new TestController(info, dummyControllerParser));
    sprinklersControllerFloor2Room2A->Subscribe(list, EventTopic(smoke, floor2Room2A));
    sprinklersControllerFloor2Room2A->Subscribe(list, EventTopic(temp, floor2Room2A));

    SharedPtr<TestController> acvControllerFloor2Room2A(new TestController(info, dummyControllerParser));
    acvControllerFloor2Room2A->Subscribe(list, EventTopic(movement, floor2Room2A));
    acvControllerFloor2Room2A->Subscribe(list, EventTopic(temp, floor2Room2A));

    SharedPtr<TestController> fireAlarmControllerFloor2Room2A(new TestController(info, dummyControllerParser));
    fireAlarmControllerFloor2Room2A->Subscribe(list, EventTopic(smoke, floor2Room2A));

    SharedPtr<TestController> sprinklersControllerFloor2Room5A(new TestController(info, dummyControllerParser));
    sprinklersControllerFloor2Room5A->Subscribe(list, EventTopic(temp, floor2Room5A));

    SharedPtr<TestController> acvControllerFloor5Room7D(new TestController(info, dummyControllerParser));
    acvControllerFloor5Room7D->Subscribe(list, EventTopic(temp, floor5Room7D));
    acvControllerFloor5Room7D->Subscribe(list, EventTopic(smoke, floor5Room7D));

    SharedPtr<Event> testedEventTempFloor2Room2A(new Event(temp, floor2Room2A, "7:30"));
    SharedPtr<Event> testedEventSmokeFloor2Room2A(new Event(smoke, floor2Room2A, "2:51"));
    SharedPtr<Event> testedEventSmokeFloor5Room7D(new Event(smoke, floor5Room7D, "2:51"));

    SubscribersList subscribedToTestedEventTempFloor2Room2A = list->SearchEventSubscribers(EventTopic(testedEventTempFloor2Room2A->GetType(), testedEventTempFloor2Room2A->GetLocation()));
    SubscribersList subscribedToTestedEventSmokeFloor2Room2A = list->SearchEventSubscribers(EventTopic(testedEventSmokeFloor2Room2A->GetType(), testedEventSmokeFloor2Room2A->GetLocation()));
    SubscribersList subscribedToTestedEventSmokeFloor5Room7D = list->SearchEventSubscribers(EventTopic(testedEventSmokeFloor5Room7D->GetType(), testedEventSmokeFloor5Room7D->GetLocation()));

    ASSERT_EQUAL(subscribedToTestedEventTempFloor2Room2A.Size(), 2);
    ASSERT_EQUAL(subscribedToTestedEventSmokeFloor2Room2A.Size(), 2);
    ASSERT_EQUAL(subscribedToTestedEventSmokeFloor5Room7D.Size(), 1);

    SubscribersListHandleEvent(subscribedToTestedEventTempFloor2Room2A, testedEventTempFloor2Room2A);
    SubscribersListHandleEvent(subscribedToTestedEventSmokeFloor2Room2A, testedEventSmokeFloor2Room2A);
    SubscribersListHandleEvent(subscribedToTestedEventSmokeFloor5Room7D, testedEventSmokeFloor5Room7D);

    ASSERT_EQUAL(sprinklersControllerFloor2Room2A->GetRightEventsNum(), 2);
    ASSERT_EQUAL(acvControllerFloor2Room2A->GetRightEventsNum(), 1);
    ASSERT_EQUAL(fireAlarmControllerFloor2Room2A->GetRightEventsNum(), 1);
    ASSERT_EQUAL(sprinklersControllerFloor2Room5A->GetRightEventsNum(), 0);
    ASSERT_EQUAL(acvControllerFloor5Room7D->GetRightEventsNum(), 1);
END_UNIT

UNIT(wild_cards_with_overlap_floor)
    AgentLocation loc("room_1_a", "1");
    AgentInformation info("light-1-a-1", "PhilipsHue", loc, "lights", "iot:1003,sat:242, bright:154, hue:8200");    SharedPtr<EventDistributionList> list(new EventDistributionList());

    EventType smoke = "SMOKE";
    EventType temp = "TEMP";
    EventType movement = "MOVE_DETECTED";
    EventLocation floor2("2");
    EventLocation floor2Room2A("2", "2A");
    EventLocation floor2Room5A("2", "5A");
    EventLocation floor5Room7D("5", "7D");

    SharedPtr<ControllerEventTopicParser> dummyControllerParser(new ControllerEventTopicParser());

    SharedPtr<TestController> sprinklersControllerFloor2(new TestController(info, dummyControllerParser));
    sprinklersControllerFloor2->Subscribe(list, EventTopic(smoke, floor2));
    sprinklersControllerFloor2->Subscribe(list, EventTopic(temp, floor2));

    SharedPtr<TestController> acvControllerFloor2Room2A(new TestController(info, dummyControllerParser));
    acvControllerFloor2Room2A->Subscribe(list, EventTopic(movement, floor2Room2A));
    acvControllerFloor2Room2A->Subscribe(list, EventTopic(temp, floor2Room2A));

    SharedPtr<TestController> fireAlarmControllerFloor2Room2A(new TestController(info, dummyControllerParser));
    fireAlarmControllerFloor2Room2A->Subscribe(list, EventTopic(smoke, floor2Room2A));

    SharedPtr<TestController> sprinklersControllerFloor2Room5A(new TestController(info, dummyControllerParser));
    sprinklersControllerFloor2Room5A->Subscribe(list, EventTopic(temp, floor2Room5A));

    SharedPtr<TestController> acvControllerFloor5Room7D(new TestController(info, dummyControllerParser));
    acvControllerFloor5Room7D->Subscribe(list, EventTopic(temp, floor5Room7D));
    acvControllerFloor5Room7D->Subscribe(list, EventTopic(smoke, floor5Room7D));

    SharedPtr<Event> testedEventTempFloor2Room2A(new Event(temp, floor2Room2A, "7:30"));
    SharedPtr<Event> testedEventSmokeFloor2Room2A(new Event(smoke, floor2Room2A, "2:51"));
    SharedPtr<Event> testedEventSmokeFloor5Room7D(new Event(smoke, floor5Room7D, "2:51"));
    SharedPtr<Event> testedEventTempFloor2Room5A(new Event(temp, floor2Room5A, "2:20"));

    SubscribersList subscribedToTestedEventTempFloor2Room2A = list->SearchEventSubscribers(EventTopic(testedEventTempFloor2Room2A->GetType(), testedEventTempFloor2Room2A->GetLocation()));
    SubscribersList subscribedToTestedEventSmokeFloor2Room2A = list->SearchEventSubscribers(EventTopic(testedEventSmokeFloor2Room2A->GetType(), testedEventSmokeFloor2Room2A->GetLocation()));
    SubscribersList subscribedToTestedEventSmokeFloor5Room7D = list->SearchEventSubscribers(EventTopic(testedEventSmokeFloor5Room7D->GetType(), testedEventSmokeFloor5Room7D->GetLocation()));
    SubscribersList subscribedToTestedEventTempFloor2Room5A = list->SearchEventSubscribers(EventTopic(testedEventTempFloor2Room5A->GetType(), testedEventTempFloor2Room5A->GetLocation()));

    ASSERT_EQUAL(subscribedToTestedEventTempFloor2Room2A.Size(), 2);
    ASSERT_EQUAL(subscribedToTestedEventSmokeFloor2Room2A.Size(), 2);
    ASSERT_EQUAL(subscribedToTestedEventSmokeFloor5Room7D.Size(), 1);
    ASSERT_EQUAL(subscribedToTestedEventTempFloor2Room5A.Size(), 2);

    SubscribersListHandleEvent(subscribedToTestedEventTempFloor2Room2A, testedEventTempFloor2Room2A);
    SubscribersListHandleEvent(subscribedToTestedEventSmokeFloor2Room2A, testedEventSmokeFloor2Room2A);
    SubscribersListHandleEvent(subscribedToTestedEventSmokeFloor5Room7D, testedEventSmokeFloor5Room7D);
    SubscribersListHandleEvent(subscribedToTestedEventTempFloor2Room5A, testedEventTempFloor2Room5A);

    ASSERT_EQUAL(sprinklersControllerFloor2->GetRightEventsNum(), 3);
    ASSERT_EQUAL(acvControllerFloor2Room2A->GetRightEventsNum(), 1);
    ASSERT_EQUAL(fireAlarmControllerFloor2Room2A->GetRightEventsNum(), 1);
    ASSERT_EQUAL(sprinklersControllerFloor2Room5A->GetRightEventsNum(), 1);
    ASSERT_EQUAL(acvControllerFloor5Room7D->GetRightEventsNum(), 1);
END_UNIT

UNIT(wild_cards_with_overlap_event_type)
    AgentLocation loc("room_1_a", "1");
    AgentInformation info("light-1-a-1", "PhilipsHue", loc, "lights", "iot:1003,sat:242, bright:154, hue:8200");    SharedPtr<EventDistributionList> list(new EventDistributionList());

    EventType smoke = "SMOKE";
    EventType temp = "TEMP";
    EventType movement = "MOVE_DETECTED";
    EventLocation floor2("2");
    EventLocation floor2Room2A("2", "2A");
    EventLocation floor2Room5A("2", "5A");
    EventLocation floor5Room7D("5", "7D");

    SharedPtr<ControllerEventTopicParser> dummyControllerParser(new ControllerEventTopicParser());

    SharedPtr<TestController> sprinklersControllerFloor2(new TestController(info, dummyControllerParser));
    sprinklersControllerFloor2->Subscribe(list, EventTopic(smoke, floor2));
    sprinklersControllerFloor2->Subscribe(list, EventTopic(temp, floor2));

    SharedPtr<TestController> acvControllerFloor2Room2A(new TestController(info, dummyControllerParser));
    acvControllerFloor2Room2A->Subscribe(list, EventTopic(movement, floor2Room2A));
    acvControllerFloor2Room2A->Subscribe(list, EventTopic(temp, floor2Room2A));

    SharedPtr<TestController> sprinklersControllerFloor2Room5A(new TestController(info, dummyControllerParser));
    sprinklersControllerFloor2Room5A->Subscribe(list, EventTopic(temp, floor2Room5A));

    SharedPtr<TestController> acvControllerFloor5Room7D(new TestController(info, dummyControllerParser));
    acvControllerFloor5Room7D->Subscribe(list, EventTopic(temp, floor5Room7D));
    acvControllerFloor5Room7D->Subscribe(list, EventTopic(smoke, floor5Room7D));

    SharedPtr<TestController> generalSprinklersController(new TestController(info, dummyControllerParser));
    generalSprinklersController->Subscribe(list, smoke);
    generalSprinklersController->Subscribe(list, temp);

    SharedPtr<Event> testedEventTempFloor2Room2A(new Event(temp, floor2Room2A, "7:30"));
    SharedPtr<Event> testedEventSmokeFloor2Room2A(new Event(smoke, floor2Room2A, "2:51"));
    SharedPtr<Event> testedEventSmokeFloor5Room7D(new Event(smoke, floor5Room7D, "2:51"));
    SharedPtr<Event> testedEventTempFloor2Room5A(new Event(temp, floor2Room5A, "2:20"));

    SubscribersList subscribedToTestedEventTempFloor2Room2A = list->SearchEventSubscribers(EventTopic(testedEventTempFloor2Room2A->GetType(), testedEventTempFloor2Room2A->GetLocation()));
    SubscribersList subscribedToTestedEventSmokeFloor2Room2A = list->SearchEventSubscribers(EventTopic(testedEventSmokeFloor2Room2A->GetType(), testedEventSmokeFloor2Room2A->GetLocation()));
    SubscribersList subscribedToTestedEventSmokeFloor5Room7D = list->SearchEventSubscribers(EventTopic(testedEventSmokeFloor5Room7D->GetType(), testedEventSmokeFloor5Room7D->GetLocation()));
    SubscribersList subscribedToTestedEventTempFloor2Room5A = list->SearchEventSubscribers(EventTopic(testedEventTempFloor2Room5A->GetType(), testedEventTempFloor2Room5A->GetLocation()));

    ASSERT_EQUAL(subscribedToTestedEventTempFloor2Room2A.Size(), 3);
    ASSERT_EQUAL(subscribedToTestedEventSmokeFloor2Room2A.Size(), 2);
    ASSERT_EQUAL(subscribedToTestedEventSmokeFloor5Room7D.Size(), 2);
    ASSERT_EQUAL(subscribedToTestedEventTempFloor2Room5A.Size(), 3);

    SubscribersListHandleEvent(subscribedToTestedEventTempFloor2Room2A, testedEventTempFloor2Room2A);
    SubscribersListHandleEvent(subscribedToTestedEventSmokeFloor2Room2A, testedEventSmokeFloor2Room2A);
    SubscribersListHandleEvent(subscribedToTestedEventSmokeFloor5Room7D, testedEventSmokeFloor5Room7D);
    SubscribersListHandleEvent(subscribedToTestedEventTempFloor2Room5A, testedEventTempFloor2Room5A);

    ASSERT_EQUAL(sprinklersControllerFloor2->GetRightEventsNum(), 3);
    ASSERT_EQUAL(acvControllerFloor2Room2A->GetRightEventsNum(), 1);
    ASSERT_EQUAL(sprinklersControllerFloor2Room5A->GetRightEventsNum(), 1);
    ASSERT_EQUAL(acvControllerFloor5Room7D->GetRightEventsNum(), 1);
    ASSERT_EQUAL(generalSprinklersController->GetRightEventsNum(), 4);
END_UNIT

UNIT(controllers_subscribed_to_same_topics)
    AgentLocation loc("room_1_a", "1");
    AgentInformation info("light-1-a-1", "PhilipsHue", loc, "lights", "iot:1003,sat:242, bright:154, hue:8200");    SharedPtr<EventDistributionList> list(new EventDistributionList());

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

    SubscribersList subscribedToTestedEventTempFloor2Room2A = list->SearchEventSubscribers(EventTopic(testedEventTempFloor2Room2A->GetType(), testedEventTempFloor2Room2A->GetLocation()));
    SubscribersList subscribedToTestedEventSmokeFloor2Room2A = list->SearchEventSubscribers(EventTopic(testedEventSmokeFloor2Room2A->GetType(), testedEventSmokeFloor2Room2A->GetLocation()));
    SubscribersList subscribedToTestedEventSmokeFloor5Room7D = list->SearchEventSubscribers(EventTopic(testedEventSmokeFloor5Room7D->GetType(), testedEventSmokeFloor5Room7D->GetLocation()));
    SubscribersList subscribedToTestedEventMovementFloor2Room2A = list->SearchEventSubscribers(EventTopic(testedEventMovementFloor2Room2A->GetType(), testedEventMovementFloor2Room2A->GetLocation()));

    ASSERT_EQUAL(subscribedToTestedEventTempFloor2Room2A.Size(), 4);
    ASSERT_EQUAL(subscribedToTestedEventSmokeFloor2Room2A.Size(), 2);
    ASSERT_EQUAL(subscribedToTestedEventSmokeFloor5Room7D.Size(), 2);
    ASSERT_EQUAL(subscribedToTestedEventMovementFloor2Room2A.Size(), 2);

    SubscribersListHandleEvent(subscribedToTestedEventTempFloor2Room2A, testedEventTempFloor2Room2A);
    SubscribersListHandleEvent(subscribedToTestedEventSmokeFloor2Room2A, testedEventSmokeFloor2Room2A);
    SubscribersListHandleEvent(subscribedToTestedEventSmokeFloor5Room7D, testedEventSmokeFloor5Room7D);
    SubscribersListHandleEvent(subscribedToTestedEventMovementFloor2Room2A, testedEventMovementFloor2Room2A);

    ASSERT_EQUAL(sprinklersControllerFloor2->GetRightEventsNum(), 2);
    ASSERT_EQUAL(acvControllerFloor2Room2A->GetRightEventsNum(), 2);
    ASSERT_EQUAL(sprinklersControllerFloor2Room2A->GetRightEventsNum(), 2);
    ASSERT_EQUAL(acvControllerFloor5Room7D->GetRightEventsNum(), 1);
    ASSERT_EQUAL(generalSprinklersController->GetRightEventsNum(), 3);
END_UNIT

UNIT(unregister_topics)
    AgentLocation loc("room_1_a", "1");
    AgentInformation info("light-1-a-1", "PhilipsHue", loc, "lights", "iot:1003,sat:242, bright:154, hue:8200");    SharedPtr<EventDistributionList> list(new EventDistributionList());

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

    SubscribersList subscribedToTestedEventTempFloor2Room2A = list->SearchEventSubscribers(EventTopic(testedEventTempFloor2Room2A->GetType(), testedEventTempFloor2Room2A->GetLocation()));
    SubscribersList subscribedToTestedEventSmokeFloor2Room2A = list->SearchEventSubscribers(EventTopic(testedEventSmokeFloor2Room2A->GetType(), testedEventSmokeFloor2Room2A->GetLocation()));
    SubscribersList subscribedToTestedEventSmokeFloor5Room7D = list->SearchEventSubscribers(EventTopic(testedEventSmokeFloor5Room7D->GetType(), testedEventSmokeFloor5Room7D->GetLocation()));
    SubscribersList subscribedToTestedEventMovementFloor2Room2A = list->SearchEventSubscribers(EventTopic(testedEventMovementFloor2Room2A->GetType(), testedEventMovementFloor2Room2A->GetLocation()));

    ASSERT_EQUAL(subscribedToTestedEventTempFloor2Room2A.Size(), 4);
    ASSERT_EQUAL(subscribedToTestedEventSmokeFloor2Room2A.Size(), 2);
    ASSERT_EQUAL(subscribedToTestedEventSmokeFloor5Room7D.Size(), 2);
    ASSERT_EQUAL(subscribedToTestedEventMovementFloor2Room2A.Size(), 2);

    SubscribersListHandleEvent(subscribedToTestedEventTempFloor2Room2A, testedEventTempFloor2Room2A);
    SubscribersListHandleEvent(subscribedToTestedEventSmokeFloor2Room2A, testedEventSmokeFloor2Room2A);
    SubscribersListHandleEvent(subscribedToTestedEventSmokeFloor5Room7D, testedEventSmokeFloor5Room7D);
    SubscribersListHandleEvent(subscribedToTestedEventMovementFloor2Room2A, testedEventMovementFloor2Room2A);

    ASSERT_EQUAL(sprinklersControllerFloor2->GetRightEventsNum(), 2);
    ASSERT_EQUAL(acvControllerFloor2Room2A->GetRightEventsNum(), 2);
    ASSERT_EQUAL(sprinklersControllerFloor2Room2A->GetRightEventsNum(), 2);
    ASSERT_EQUAL(acvControllerFloor5Room7D->GetRightEventsNum(), 1);
    ASSERT_EQUAL(generalSprinklersController->GetRightEventsNum(), 3);

    SharedPtr<Event> testedEventTempFloor5Room7D(new Event(temp, floor5Room7D, "7:30"));
    SubscribersList subscribedToTestedFirstEventTempFloor5Room7D = list->SearchEventSubscribers(EventTopic(testedEventTempFloor5Room7D->GetType(), testedEventTempFloor5Room7D->GetLocation()));
    SubscribersListHandleEvent(subscribedToTestedFirstEventTempFloor5Room7D, testedEventTempFloor5Room7D);

    ASSERT_EQUAL(sprinklersControllerFloor2->GetRightEventsNum(), 2);
    ASSERT_EQUAL(acvControllerFloor2Room2A->GetRightEventsNum(), 2);
    ASSERT_EQUAL(sprinklersControllerFloor2Room2A->GetRightEventsNum(), 2);
    ASSERT_EQUAL(acvControllerFloor5Room7D->GetRightEventsNum(), 2);
    ASSERT_EQUAL(generalSprinklersController->GetRightEventsNum(), 4);

    list->Unregister(generalSprinklersController);

    SubscribersList subscribedToTestedSecondEventTempFloor5Room7D = list->SearchEventSubscribers(EventTopic(testedEventTempFloor5Room7D->GetType(), testedEventTempFloor5Room7D->GetLocation()));
    SubscribersListHandleEvent(subscribedToTestedSecondEventTempFloor5Room7D, testedEventTempFloor5Room7D);

    ASSERT_EQUAL(sprinklersControllerFloor2->GetRightEventsNum(), 2);
    ASSERT_EQUAL(acvControllerFloor2Room2A->GetRightEventsNum(), 2);
    ASSERT_EQUAL(sprinklersControllerFloor2Room2A->GetRightEventsNum(), 2);
    ASSERT_EQUAL(acvControllerFloor5Room7D->GetRightEventsNum(), 3);
    ASSERT_EQUAL(generalSprinklersController->GetRightEventsNum(), 4);
END_UNIT

TEST_SUITE(event_distribution_list)
    TEST(no_overlap_no_wild_cards)
    TEST(wild_cards_no_overlap)
    TEST(wild_cards_with_overlap_room)
    TEST(wild_cards_with_overlap_floor)
    TEST(wild_cards_with_overlap_event_type)
    TEST(controllers_subscribed_to_same_topics)
    TEST(unregister_topics)
END_SUITE