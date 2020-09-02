#include "test_utils.hpp"

namespace smarthome
{

SharedPtr<EventMiddleWare> MakeMiddleWare(size_t a_capacity)
{
    return SharedPtr<EventMiddleWare>(new EventMiddleWare(a_capacity));
}

SharedPtr<TestSensor> MakeSensor(const AgentInformation& a_agentInfo, const SharedPtr<IEventTypeParser>& a_parser, size_t a_numOfEvents)
{
    return SharedPtr<TestSensor>(new TestSensor(a_agentInfo, a_parser, a_numOfEvents));
}

SharedPtr<HubThread> MakeHubThread(const SharedPtr<IEventDistributer>& a_eventSender, const SharedPtr<IEventPuller>& a_eventPuller, const SharedPtr<ISearch>& a_subscribersSearcher, size_t a_numOfEvents)
{
    SharedPtr<TestHub> hub(new TestHub(a_eventSender, a_eventPuller, a_subscribersSearcher, a_numOfEvents));
    return SharedPtr<HubThread>(new HubThread(hub));
}

SharedPtr<ChannelsContainer> MakeChannels()
{
    SharedPtr<ChannelsContainer> channels(new ChannelsContainer());
    channels->reserve(NUM_OF_CHANNELS);

    for(int i = 0; i < NUM_OF_CHANNELS; ++i)
    {
        SharedPtr<EventChannel> eventChannel(new EventChannel());
        channels->push_back(eventChannel);
    }

    return channels;
}

SharedPtr<EventPackagePushers> MakePackagePushers(const SharedPtr<ChannelsContainer>& a_channels)
{
    size_t numOfChannels = a_channels->size();
    SharedPtr<EventPackagePushers> pushers(new EventPackagePushers());
    pushers->reserve(numOfChannels);

    for(size_t i = 0; i < numOfChannels; ++i)
    {
        SharedPtr<EventChannel> channel((*a_channels)[i]);
        SharedPtr<IEventPackagePusher> channelPusher(std::tr1::static_pointer_cast<IEventPackagePusher>(channel));
        pushers->push_back(channelPusher);
    }

    return pushers;
}

SharedPtr<IEventPackagePuller> MakePackagePuller(const SharedPtr<EventChannel>& a_channel)
{
    return std::tr1::static_pointer_cast<IEventPackagePuller>(a_channel);
}

SharedPtr<Hub> MakeHub(const SharedPtr<IEventDistributer>& a_eventSender, const SharedPtr<IEventPuller>& a_eventPuller, const SharedPtr<ISearch>& a_subscribersSearcher)
{
    return SharedPtr<Hub>(new Hub(a_eventSender, a_eventPuller, a_subscribersSearcher));
}

} //namespace smarthome