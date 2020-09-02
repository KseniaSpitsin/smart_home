#ifndef HUB_THREAD_HPP
#define HUB_THREAD_HPP

#include "common_utils.hpp"
#include "r_thread.hpp"
#include "test_hub.hpp"

namespace smarthome
{

class HubThread : private advcpp::Uncopyable
{
public:
    HubThread(const SharedPtr<TestHub>& a_hunRunnable);
    //~HubThread() = default;

    void ShutDown();

private:
    SharedPtr<TestHub> m_hub; //has to be before m_thread
    advcpp::RThread m_thread;
};

} //namespace smarthome

#endif //HUB_THREAD_HPP