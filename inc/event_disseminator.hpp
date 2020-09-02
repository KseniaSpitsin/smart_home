#ifndef EVENT_DISSEMINATOR_HPP
#define EVENT_DISSEMINATOR_HPP

#include <vector>

#include "common_utils.hpp"
#include "i_event_distributer.hpp"

namespace smarthome
{

class IEventPackagePusher;

class EventDisseminator : public IEventDistributer
{
public:
    typedef std::vector<SharedPtr<IEventPackagePusher> > Channels;

public:
    EventDisseminator(const Channels& a_channels);
    virtual ~EventDisseminator() NOEXCEPTIONS;

    virtual void Deliver(const SharedPtr<Event>& a_event, SubscribersList a_list) NOEXCEPTIONS;
    virtual void PublicAll(const EventPackage& a_package);

private:
    Channels m_channels;
};

} //namespace smarthome

#endif //EVENT_DISSEMINATOR_HPP