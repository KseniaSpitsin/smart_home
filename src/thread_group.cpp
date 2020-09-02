#include "thread_group.hpp"

namespace smarthome
{

ThreadGroup::ThreadGroup(const TasksContainer& a_tasks)
    : m_tasks(a_tasks)
    , m_threads()
{
    size_t numOfTasks = a_tasks.size();
    m_threads.reserve(numOfTasks);

    for(size_t i = 0; i < numOfTasks; ++i)
    {
        SharedPtr<advcpp::RThread> newThread(new advcpp::RThread(*a_tasks[i]));
        m_threads.push_back(newThread);
    }
}

void ThreadGroup::Join()
{
    size_t numOfThreads = m_threads.size();

    for(size_t i = 0; i < numOfThreads; ++i)
    {
        m_threads[i]->Join();
    }
}

} //namespace smarthome