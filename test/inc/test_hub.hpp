#ifndef TEST_HUB_HPP
#define TEST_HUB_HPP

#include "common_utils.hpp"
#include "runnable.hpp"

namespace smarthome
{

class IEventDistributer;
class IEventPuller;
class ISearch;

class TestHub : public advcpp::Runnable
{
public:
    TestHub(const SharedPtr<IEventDistributer>& a_eventSender , const SharedPtr<IEventPuller>& a_eventPuller, const SharedPtr<ISearch>& a_subscribersSearcher, size_t a_eventNum);
    virtual ~TestHub() NOEXCEPTIONS;

    void ShutDown();

private:
    virtual void Run() NOEXCEPTIONS;

private:
    SharedPtr<IEventDistributer> m_eventSender;
    SharedPtr<IEventPuller> m_eventPuller;
    SharedPtr<ISearch> m_searcher;
    size_t m_numOfEvents;
};

} //namespace smarthome

#endif //TEST_HUB_HPP