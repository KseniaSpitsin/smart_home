#ifndef EVENT_MIDDLE_WARE_HPP
#define EVENT_MIDDLE_WARE_HPP

#include "i_event_pusher.hpp"
#include "i_event_puller.hpp"
#include "waitable_bounded_queue.hpp"

namespace smarthome
{

class EventMiddleWare : public IEventPusher, public IEventPuller
{
public:
    EventMiddleWare(size_t a_capacity);
    virtual ~EventMiddleWare() NOEXCEPTIONS;

    virtual void Push(const SharedPtr<Event>& a_event);
    virtual void Pull(SharedPtr<Event>& a_event);

    void ShutDown() NOEXCEPTIONS;

private:
    typedef advcpp::WaitableBoundedQueue<SharedPtr<Event> > EventContainer;

private:
    EventContainer m_container;
};

} //namespace smarthome

#endif //EVENT_MIDDLE_WARE_HPP