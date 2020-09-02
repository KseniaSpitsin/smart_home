#ifndef EVENT_PACKAGE_HPP
#define EVENT_PACKAGE_HPP

#include <utility>
#include "common_utils.hpp"
#include "event.hpp"

namespace smarthome
{

class IEventHandler;

typedef std::pair<SharedPtr<IEventHandler>, SharedPtr<Event> > EventPackage;

} //namespace smarthome

#endif //EVENT_PACKAGE_HPP