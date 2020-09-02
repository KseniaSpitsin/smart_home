#ifndef MUTEX_HPP
#define MUTEX_HPP

#include <pthread.h>

#include "common_utils.hpp"

namespace advcpp
{

//!@brief Mutex designed to prevent more then one thread enter the critical
// sections by using the locking mechanism. Mutex provide exclusive
// ownership and do not support recursive locking.

class Mutex : private Uncopyable
{
private:
    enum From
    {
        FROM_CTOR = 0,
        FROM_DTOR,
        FROM_LOCK,
        FROM_UNLOCK
    };

public:
    Mutex();
    ~Mutex() NOEXCEPTIONS;

    void Lock();
    void Unlock();

private:
    friend class CondVar;
    
    pthread_mutex_t* getNativeMutex();
    int tryInitMutex();
    void handleError(int a_errorCode, From a_fromFunction);

private:
    pthread_mutex_t m_mutex;
};

} //advcpp

#endif //MUTEX_HPP