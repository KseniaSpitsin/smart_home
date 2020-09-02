#include <exception>
#include <cassert>

#include "common_utils.hpp"
#include "mutex.hpp"

namespace advcpp
{

unsigned long MilliToMicro(unsigned int a_miliSec)
{
    return a_miliSec * 1000;
}

unsigned long MicroToNano(unsigned int a_microSec)
{
    return a_microSec * 1000;
}

Unassignable::Unassignable() NOEXCEPTIONS
{
}

Unassignable::~Unassignable() NOEXCEPTIONS
{
}

Unassignable::Unassignable(const Unassignable& a_other) NOEXCEPTIONS
{
}

Uncopyable::Uncopyable() NOEXCEPTIONS
: Unassignable()
{
}

Uncopyable::~Uncopyable() NOEXCEPTIONS
{
}

LockGuard::LockGuard(Mutex& a_mutex)
: m_mutex(a_mutex)
{
    m_mutex.Lock();
}

LockGuard::~LockGuard() NOEXCEPTIONS
{
    try
    {
        m_mutex.Unlock();   
    }
    catch(const std::exception& a_exception)
    {
        a_exception.what();
        assert(!"Error on unlocking the mutex.");
    }
}

AtomicValue<bool>::AtomicValue(bool a_val) NOEXCEPTIONS
    : m_val(a_val)
{
}

bool AtomicValue<bool>::Acquire() NOEXCEPTIONS
{
    return __sync_lock_test_and_set(&m_val, true);
}

bool AtomicValue<bool>::Release() NOEXCEPTIONS
{
    return __sync_lock_test_and_set(&m_val, false);
}

bool AtomicValue<bool>::Get() const NOEXCEPTIONS
{
    return __sync_lock_test_and_set(const_cast<bool*>(&m_val), m_val);
}

SoLibHandler::SoLibHandler(const char* filename, int flags)
{
    m_handle = dlopen(filename, flags);
    if(m_handle == 0)
    {
        throw dlerror();
    }
}

SoLibHandler::~SoLibHandler()
{
    dlclose(m_handle);
}

} //advcpp