#ifndef I_EVENT_PACKAGE_PULLER_HPP
#define I_EVENT_PACKAGE_PULLER_HPP

#include "common_utils.hpp"
#include "event_package.hpp"

namespace smarthome
{
    
class IEventPackagePuller : private advcpp::Uncopyable
{
public:
    //IEventPackagePuller() = default;
    virtual ~IEventPackagePuller() NOEXCEPTIONS;

    //!@exception throws EventPackagePullerShutDown on shut down
    virtual void Pull(EventPackage& a_info) = 0;
};

} //smarthome

#endif //I_EVENT_PACKAGE_PULLER_HPP