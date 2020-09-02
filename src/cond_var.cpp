#include <cerrno>
#include <string>
#include <unistd.h>

#include "cond_var.hpp"

namespace advcpp
{

static const std::string g_errorFrom[] = { "CondVar Ctor error.", "CondVar Dtor error."
                                        , "CondVar Wait error."};
static const int g_maxNumOfTrys = 3;

CondVar::CondVar()
{
    int statusCode = tryInitCondVar();
    if(0 != statusCode)
    {
        handleError(statusCode, FROM_CTOR);
    }
}

CondVar::~CondVar() NOEXCEPTIONS
{
    int statusCode = pthread_cond_destroy(&m_condVar);
    if(0 != statusCode)
    {
        handleError(statusCode, FROM_DTOR);
    }
}

void CondVar::NotifyOne() NOEXCEPTIONS
{
    int statusCode = pthread_cond_signal(&m_condVar);

    if(0 != statusCode)
    {
        assert(!"CondVar NotifyOne error. Check if condition variable initialized in ctor");
    }
}

void CondVar::NotifyAll() NOEXCEPTIONS
{
    int statusCode = pthread_cond_broadcast(&m_condVar);

    if(0 != statusCode)
    {
        assert(!"CondVar NotifyAll error. Check if condition variable initialized in ctor");
    }
}

int CondVar::tryInitCondVar()
{
    unsigned int miliSecUntilRetry = 4;
    int numberOfTrys = 0;
    int statusCode;

    while(numberOfTrys != g_maxNumOfTrys)
    {
        statusCode = pthread_cond_init(&m_condVar, 0);
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

void CondVar::notifyAllAndDestroy()
{
    do
    {
        NotifyAll();
    } while(0 != pthread_cond_destroy(&m_condVar));
}

void CondVar::handleError(int a_errorCode, From a_fromFunc)
{
    switch(a_errorCode)
    {
        case EAGAIN:
            throw CondVarLackOfSources();
            break;
        case ENOMEM:
            throw CondVarLackOfMemory();
            break;
        case EBUSY:
            if(FROM_CTOR == a_fromFunc)
            {
                assert(!"CondVar ctor error. Check if called init twice on same cond var");
            }
            else
            {
                notifyAllAndDestroy();
            }
            break;
        case EINVAL:
            {
                std::string errorMsg = g_errorFrom[a_errorCode] + " Attributes in init are invalid";
                assert(!errorMsg.c_str());
            }
            break;
        case EPERM:
            throw CondVarOwnershipError();
            break;
        default:
            assert(!"Undocumented CondVar error accoured");
            break;
    }
}

} //advcpp