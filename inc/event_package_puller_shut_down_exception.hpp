#ifndef EVENT_PACKAGE_PULLER_SHUT_DOWN_EXCEPTION
#define EVENT_PACKAGE_PULLER_SHUT_DOWN_EXCEPTION

#include <exception>
#include "common_utils.hpp"

namespace smarthome
{

class EventPackagePullerShutDownException : public std::exception
{
public:
    virtual const char* what() const NOEXCEPTIONS;
};

} //namespace smarthome

#endif //EVENT_PACKAGE_PULLER_SHUT_DOWN_EXCEPTION