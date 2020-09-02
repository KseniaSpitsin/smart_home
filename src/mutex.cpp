#include <cassert>
#include <cerrno>
#include <exception>
#include <string>
#include <unistd.h>

#include "mutex.hpp"
#include "mutex_exceptions.hpp"

namespace advcpp
{

static const size_t g_maxNumOfTrys = 3;
static const std::string g_fromErrors[] = {"Mutex Ctor error.", "Mutex Dtor error."
                                            , "Mutex Lock error.", "Mutex Unlock error."};

Mutex::Mutex()
{
    int statusCode = tryInitMutex();
    if(0 != statusCode)
    {
        handleError(statusCode, FROM_CTOR);
    }
}

Mutex::~Mutex() NOEXCEPTIONS
{
    int statusCode = pthread_mutex_destroy(&m_mutex);
    if(0 != statusCode)
    {
        handleError(statusCode, FROM_DTOR);
    }
}

void Mutex::Lock()
{
    int statusCode = pthread_mutex_lock(&m_mutex);
    if(0 != statusCode)
    {
        handleError(statusCode, FROM_LOCK);
    }
}

void Mutex::Unlock()
{
    int statusCode = pthread_mutex_unlock(&m_mutex);
    if(0 != statusCode)
    {
        handleError(statusCode, FROM_UNLOCK);
    }
}

pthread_mutex_t* Mutex::getNativeMutex()
{ 
    return &m_mutex; 
}

int Mutex::tryInitMutex()
{
    unsigned int miliSecUntilRetry = 4;
    size_t numberOfTrys = 0;
    int statusCode;

    while(numberOfTrys != g_maxNumOfTrys)
    {
        statusCode = pthread_mutex_init(&m_mutex, 0);
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

void Mutex::handleError(int a_errorCode, From a_fromFunction)
{
    switch(a_errorCode)
    {
        case EAGAIN:
            if(FROM_CTOR == a_fromFunction)
            {
                throw MutexLackOfSource();
            }
            else
            {
                 throw MutexMaxRecursive();
            }
            break;
        case EPERM:
            if(FROM_CTOR == a_fromFunction)
            {
                assert(!"Mutex ctor error. Check the attributes.");
            }
            else
            {
                throw MutexOwnershipError();
            }
            break;
        case EBUSY:
            if(FROM_CTOR == a_fromFunction)
            {
                assert(!"Mutex ctor error. Check if called init twice on same mutex.");
            }
            else
            {
                assert(!"Mutex dtor error. Are you trying to destroy a mutex while in use?");
            }
            break;
        case EINVAL:
            {
                std::string errorMsg = g_fromErrors[a_errorCode] + " Attributes in ctor are invalid.";
                assert(!(errorMsg.c_str()));
            }
            break;
        case EDEADLK:
            throw MutexLockAgainError();
            break;
        default:
            assert(!"Undocumented mutex error");
            break;
    }
}

} //advcpp