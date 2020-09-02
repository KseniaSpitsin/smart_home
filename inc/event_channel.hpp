#ifndef EVENT_CHANNEL_HPP
#define EVENT_CHANNEL_HPP

#include "common_utils.hpp"
#include "i_event_package_pusher.hpp"
#include "i_event_package_puller.hpp"
#include "waitable_bounded_queue.hpp"
#include "event_emissary.hpp"
#include "r_thread.hpp"

namespace smarthome
{

class EventChannel : public IEventPackagePuller, public IEventPackagePusher
{
public:
    EventChannel();
    virtual ~EventChannel() NOEXCEPTIONS;

    virtual void Push(const EventPackage& a_info);
    virtual void Pull(EventPackage& a_info);

private:
    typedef advcpp::WaitableBoundedQueue<EventPackage> Channel;

private:
    Channel m_eventChannel;
};

} //namespace smarthome

#endif //EVENT_CHANNEL_HPP