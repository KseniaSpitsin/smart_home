#ifndef HUB_HPP
#define HUB_HPP

#include "common_utils.hpp"
#include "runnable.hpp"
#include "i_search.hpp"
#include "i_event_puller.hpp"
#include "i_event_distributer.hpp"
#include "r_thread.hpp"

namespace smarthome
{

class Hub : private advcpp::Runnable
{
public:
    Hub(const SharedPtr<IEventDistributer>& a_eventSender , const SharedPtr<IEventPuller>& a_eventPuller, const SharedPtr<ISearch>& a_subscribersSearcher) NOEXCEPTIONS;
    virtual ~Hub() NOEXCEPTIONS;

    virtual void ShutDown();

private:
    virtual void Run() NOEXCEPTIONS;

private:
    SharedPtr<IEventDistributer> m_eventSender;
    SharedPtr<IEventPuller> m_eventPuller;
    SharedPtr<ISearch> m_searcher;
    SharedPtr<advcpp::RThread> m_thread;
    advcpp::AtomicFlag m_isRunning;
};

} //namespace smarthome

#endif //HUB_HPP