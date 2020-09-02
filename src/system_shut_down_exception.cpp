#include "system_shut_down_exception.hpp"

namespace smarthome
{

SystemShutDownException::SystemShutDownException()
{
}

const char* SystemShutDownException::what() const NOEXCEPTIONS
{
    return "The system is shutting down";
}

} //namespace smarthome