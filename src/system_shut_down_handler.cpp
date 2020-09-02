#include "system_shut_down_handler.hpp"
#include "system_shut_down_exception.hpp"

namespace smarthome
{

SystemShutDownHandler::~SystemShutDownHandler() NOEXCEPTIONS
{
}

void SystemShutDownHandler::HandleEvent(const SharedPtr<Event>& a_event)
{
    throw SystemShutDownException();
}

} //namespace smarthome