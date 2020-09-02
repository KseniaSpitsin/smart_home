#include <exception>
#include <iostream>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <unistd.h>

#include "r_thread.hpp"
#include "runnable.hpp"
#include "thread_exceptions.hpp"

namespace advcpp 
{

static void setTimeout(struct timespec& m_timeout, size_t m_timeoutMicroSec)
{
    if(-1 == clock_gettime(CLOCK_MONOTONIC, &m_timeout))
    {
        std::cerr << std::strerror(errno) << '\n';
        assert(!"Error on clock_gettime");
    }

    m_timeout.tv_nsec += MicroToNano(m_timeoutMicroSec);
}

static const size_t g_maxNumOfTrys = 3;

void* RThread::callBack(void* a_object) NOEXCEPTIONS
{
    assert(a_object);

    Runnable* runnable = reinterpret_cast<Runnable*>(a_object);

    try
    {
        runnable->Run();
    }
    catch(const std::exception& a_exception)
    {
        std::cerr << a_exception.what();
        assert(!"Sorry, your runnable is not safe");
    }
    catch(...)
    {
        assert(!"What are you doing? O-O");
    }

    return 0;
}

RThread::RThread(Runnable& a_runnable) 
    : Uncopyable()
    , m_work(a_runnable)
    , m_joinable(true)
{
    int statusCode = tryCreateThread();

    if(0 != statusCode)
    {
        handleError(statusCode, FROM_CTOR);
    }
}

RThread::~RThread() NOEXCEPTIONS
{
    if(m_joinable)
    {
        assert(!"Error: The thread was nither detached nor joined");
    }
}

void RThread::Detach()
{
    if(!m_joinable)
    {
        return;
    }
    
    int statusCode = pthread_detach(m_threadId);

    if(0 != statusCode)
    {
        handleError(statusCode, FROM_DETACH);
    }
    else
    {
        setAsNotJoinable();
    }
}

void RThread::Join()
{
    if(!m_joinable)
    {
        return;
    }

    int statusCode = pthread_join(m_threadId, 0);

    if(0 != statusCode)
    {
        handleError(statusCode, FROM_JOIN);
    }
    else
    {
        setAsNotJoinable();
    }
}

bool RThread::TryTimedJoin(size_t a_microSec)
{
    if(!m_joinable)
    {
        return true;
    }

    struct timespec waitTime;
    setTimeout(waitTime, a_microSec);

    int statusCode = pthread_timedjoin_np(m_threadId, 0, &waitTime);

    if(0 != statusCode)
    {
        if(ETIMEDOUT == statusCode)
        {
            return false;
        }
        handleError(statusCode, FROM_TRY_JOIN);
    }

    setAsNotJoinable();
    return true;
}

int RThread::tryCreateThread() NOEXCEPTIONS
{
    unsigned int miliSecUntilRetry = 4;
    size_t numberOfTrys = 0;
    int statusCode;

    while(numberOfTrys != g_maxNumOfTrys)
    {
        statusCode = pthread_create(&m_threadId, NULL, callBack, &m_work);
        if(EAGAIN != statusCode)
        {
            return statusCode;
        }
        usleep(MilliToMicro(miliSecUntilRetry));
        ++numberOfTrys;
        miliSecUntilRetry *= 2;
    }

    return statusCode;
}

void RThread::handleError(int a_statusCode, From a_fromFunc)
{
    switch(a_statusCode)
    {
        case EAGAIN:
            throw ThreadLackOfResource();
            break;
        case EINVAL:
            if(a_fromFunc == FROM_CTOR)
            {
                assert(!"Invalid attributes. Check the ctor");
            }
            else
            {
                assert(!"Thread is not a joinable thread. Check the attr in ctor");
            }
            break;
        case EPERM:
            assert(!"Error in attributes. Check the ctor");
            break;
        case ESRCH:
            assert(!"No thread id was found. Check the ctor");
            break;
        case EDEADLK:
            throw ThreadDeadlock();
            break;
        default:
            assert(!"Undocumented thread error");
            break;
    }
}

void RThread::setAsNotJoinable() NOEXCEPTIONS
{
    m_joinable = false;
}

}