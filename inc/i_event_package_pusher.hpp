#ifndef I_EVENT_PACKAGE_PUSHER_HPP
#define I_EVENT_PACKAGE_PUSHER_HPP

#include "common_utils.hpp"
#include "event_package.hpp"

namespace smarthome
{

class IEventPackagePusher : private advcpp::Uncopyable
{
public:
    //IEventPackagePusher() = default;
    virtual ~IEventPackagePusher() NOEXCEPTIONS;

    virtual void Push(const EventPackage& a_package) = 0;
};

} //namespace smarthome

#endif //I_EVENT_PACKAGE_PUSHER_HPP