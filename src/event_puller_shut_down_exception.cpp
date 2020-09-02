#include "event_puller_shut_down_exception.hpp"

namespace smarthome
{

const char* EventPullerShutDownException::what() const NOEXCEPTIONS
{
    return "Event puller is shutting down!";
}

} //namespace smarthome