#include "event_package_puller_shut_down_exception.hpp"

namespace smarthome
{

const char* EventPackagePullerShutDownException::what() const NOEXCEPTIONS
{
    return "Event package puller is shutting down";
}

} //namespace smarthome
