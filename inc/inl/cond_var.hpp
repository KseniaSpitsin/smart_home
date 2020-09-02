#ifndef COND_VAR_INL_HPP
#define COND_VAR_INL_HPP

template<class Predicate>
void CondVar::Wait(Mutex& a_mutex, Predicate a_predicate)
{
    int statusCode;
    while(a_predicate())
    {
        statusCode = pthread_cond_wait(&m_condVar, a_mutex.getNativeMutex());
        if(0 != statusCode)
        {
            handleError(statusCode, FROM_WAIT);
        }
    }
}

#endif //COND_VAR_INL_HPP