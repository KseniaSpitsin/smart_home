#include "hub_thread.hpp"
#include "event.hpp"

namespace smarthome
{

HubThread::HubThread(const SharedPtr<TestHub>& a_hunRunnable)
    : advcpp::Uncopyable()
    , m_hub(a_hunRunnable)
    , m_thread(*m_hub)
{
}

void HubThread::ShutDown()
{
    m_thread.Join();
    m_hub->ShutDown();
}

} //namespace smarthome