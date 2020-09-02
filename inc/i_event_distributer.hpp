#ifndef I_EVENT_DISTRIBUTER_HPP
#define I_EVENT_DISTRIBUTER_HPP

#include "common_utils.hpp"
#include "subscribers_list.hpp"
#include "event_package.hpp"

namespace smarthome
{

class Event;

class IEventDistributer : private advcpp::Uncopyable
{
public:
    //IEventDistributer() = default;
    virtual ~IEventDistributer();

    virtual void Deliver(const SharedPtr<Event>& a_event, SubscribersList a_list) = 0;
    virtual void PublicAll(const EventPackage& a_package) = 0;
};

} //namespace smarthome

#endif //I_EVENT_DISTRIBUTER_HPP