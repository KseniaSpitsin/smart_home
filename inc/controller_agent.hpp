#ifndef CONTROLLER_AGENT_HPP
#define CONTROLLER_AGENT_HPP

#include <vector>

#include "i_controller.hpp"
#include "event_distribution_list.hpp"
#include "event.hpp"

namespace smarthome
{

class Event;

class ControllerAgent : public IContoller
{
public:
    typedef std::vector<SharedPtr<EventType> > EventsSubscribed;

public:
    ControllerAgent(const SharedPtr<EventsSubscribed>& a_events);

    void Subscribe(EventDistributionList& a_list);
    void HandleEvent(const Event& a_event);
};

} // namespace smart_home

#endif //CONTROLLER_AGENT_HPP