#include "event_package_pusher_shut_down_exception.hpp"

namespace smarthome
{

const char* EventPackagePusherShutDownException::what() const NOEXCEPTIONS
{
    return "Event package puller is shuting down";
}

} //namespace smarthome