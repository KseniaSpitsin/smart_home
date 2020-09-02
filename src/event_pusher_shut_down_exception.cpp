#include "event_pusher_shut_down_exception.hpp"

namespace smarthome
{

const char* EventPusherShutDownException::what() const NOEXCEPTIONS
{
    return "Event pusher is shutting down!";
}

} //namespace smarthome