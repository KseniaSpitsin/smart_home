#ifndef COND_VAR_HPP
#define COND_VAR_HPP

#include <cassert>
#include <cerrno>
#include <pthread.h>

#include "common_utils.hpp"
#include "cond_var_exceptions.hpp"
#include "mutex.hpp"

namespace advcpp
{

class CondVar : private Uncopyable
{
private:
    enum From
    {
        FROM_CTOR,
        FROM_DTOR,
        FROM_WAIT
    };

public:
    CondVar();
    ~CondVar() NOEXCEPTIONS;

    template<class Predicate>
    //Predicate - has bool operator()()
    //Nullary boolean function object or function
    void Wait(Mutex& a_mutex, Predicate a_predicate);

    void NotifyOne() NOEXCEPTIONS;
    void NotifyAll() NOEXCEPTIONS;

private:
    int tryInitCondVar();
    void notifyAllAndDestroy();
    void handleError(int a_errorCode, From a_fromFunc);

    pthread_cond_t m_condVar;
};

#include "inl/cond_var.hpp"

} //advcpp


#endif //COND_VAR_HPP