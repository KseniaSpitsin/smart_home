#ifndef THREAD_GROUP_HPP
#define THREAD_GROUP_HPP

#include <vector>
#include "common_utils.hpp"
#include "runnable.hpp"
#include "r_thread.hpp"

namespace smarthome
{

class ThreadGroup : private advcpp::Uncopyable
{
public:
    typedef std::vector<SharedPtr<advcpp::Runnable> > TasksContainer;
    typedef std::vector<SharedPtr<advcpp::RThread> > WorkingThreads;

public:
    explicit ThreadGroup(const TasksContainer& a_tasks);
    //~ThreadGroup();

    void Join();

private:
    TasksContainer m_tasks;
    WorkingThreads m_threads;
};

} //namespace smarthome

#endif //THREAD_GROUP_HPP