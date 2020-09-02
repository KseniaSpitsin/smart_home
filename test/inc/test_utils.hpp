#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

#include <vector>
#include "common_utils.hpp"
#include "agent_information.hpp"
#include "test_sensor.hpp"
#include "i_event_pusher.hpp"
#include "event_middle_ware.hpp"
#include "r_thread.hpp"
#include "event.hpp"
#include "test_hub.hpp"
#include "hub_thread.hpp"
#include "event_channel.hpp"
#include "event_channel_configuration.hpp"
#include "event_emissary.hpp"
#include "i_event_type_parser.hpp"
#include "hub.hpp"

namespace smarthome
{

typedef std::vector<SharedPtr<EventChannel> > ChannelsContainer;
typedef std::vector<SharedPtr<IEventPackagePusher> > EventPackagePushers;

SharedPtr<ChannelsContainer> MakeChannels();
SharedPtr<EventPackagePushers> MakePackagePushers(const SharedPtr<ChannelsContainer>& a_channels);

SharedPtr<IEventPackagePuller> MakePackagePuller(const SharedPtr<EventChannel>& a_channel);

SharedPtr<EventMiddleWare> MakeMiddleWare(size_t a_capacity);

SharedPtr<TestSensor> MakeSensor(const AgentInformation& a_agentInfo, const SharedPtr<IEventTypeParser>& a_parser, size_t a_numOfEvents);

SharedPtr<HubThread> MakeHubThread(const SharedPtr<IEventDistributer>& a_eventSender, const SharedPtr<IEventPuller>& a_eventPuller, const SharedPtr<ISearch>& a_subscribersSearcher, size_t a_numOfEvents);

SharedPtr<Hub> MakeHub(const SharedPtr<IEventDistributer>& a_eventSender, const SharedPtr<IEventPuller>& a_eventPuller, const SharedPtr<ISearch>& a_subscribersSearcher);

} //namespace smarthome

#endif //TEST_UTILS_HPP