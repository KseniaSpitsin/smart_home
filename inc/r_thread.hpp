#ifndef __R_THREAD_HPP__
#define __R_THREAD_HPP__

#include <ctime>
#include <pthread.h>

#include "common_utils.hpp"

namespace advcpp 
{
class Runnable;

class RThread : private Uncopyable
{
private:
    enum From
    {
        FROM_CTOR,
        FROM_JOIN,
        FROM_DETACH,
        FROM_TRY_JOIN
    };

public:
    RThread(Runnable& a_runnable);
    ~RThread() NOEXCEPTIONS;
    
    //!@brief join with a terminated thread.
    void Join();

    //!@brief detach a thread.
    void Detach();

    //!@brief Performs a join-with-timeout:
    // If thread has not yet terminated, then the call blocks until a maximum time,
    // specified in a_waitTime.
    //!@retval true if the join succeeded within a_waitTime, false otherwise.
    bool TryTimedJoin(size_t a_microSec);

private:
    static void* callBack(void* a_object) NOEXCEPTIONS;

private:
    int tryCreateThread() NOEXCEPTIONS;
    void handleError(int a_statusCode, From a_fromFunc);
    void setAsNotJoinable() NOEXCEPTIONS;

private:
    Runnable& m_work;
    bool m_joinable;
    pthread_t m_threadId;
};

} //advcpp

#endif//__R_THREAD_HPP__