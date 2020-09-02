#ifndef COMMON_UTILS_HPP
#define COMMON_UTILS_HPP
#include <tr1/unordered_set>
#include <tr1/memory> // post 2003 : ~= 2004
#include <tr1/unordered_map>
#include <dlfcn.h>
#include <iostream>

namespace advcpp 
{

#define SharedPtr std::tr1::shared_ptr

#define NOEXCEPTIONS throw()

class Mutex;

class Unassignable
{
protected:
    Unassignable() NOEXCEPTIONS;
    ~Unassignable() NOEXCEPTIONS;
    Unassignable(const Unassignable& a_other) NOEXCEPTIONS;
private:
    Unassignable& operator=(const Unassignable& a_other);
};

class Uncopyable : private Unassignable
{
protected:
    Uncopyable() NOEXCEPTIONS;
    ~Uncopyable() NOEXCEPTIONS;
private:
    Uncopyable(const Uncopyable& a_other);
};

class LockGuard
{
public:
    LockGuard(Mutex& a_mutex);
    ~LockGuard() NOEXCEPTIONS;
private:
    Mutex& m_mutex;
};

unsigned long MilliToMicro(unsigned int a_miliSec);
unsigned long MicroToNano(unsigned int a_microSec);

template<typename T>
// require for T:
    // T()
    // T is an integer type
class AtomicValue : private Uncopyable
{
public:
    explicit AtomicValue(T a_val = T()) NOEXCEPTIONS;
    // ~AtomicValue() = default;
    AtomicValue& operator=(const AtomicValue& a_other) NOEXCEPTIONS;

    T operator++() NOEXCEPTIONS;
    T operator--() NOEXCEPTIONS;
    void operator+=(T a_val) NOEXCEPTIONS;
    void operator-=(T a_val) NOEXCEPTIONS;
    T Value() const NOEXCEPTIONS;
    void Value(T a_val) NOEXCEPTIONS;

private:
    T m_value;
};

template<>
class AtomicValue<bool> : private Uncopyable
{
public:
    AtomicValue(bool a_val = false) NOEXCEPTIONS;   
    bool Acquire() NOEXCEPTIONS;
    bool Release() NOEXCEPTIONS;
    bool Get() const NOEXCEPTIONS;

private:
    bool m_val;
};

typedef AtomicValue<bool> AtomicFlag;

template<typename T>
AtomicValue<T>::AtomicValue(T a_val) NOEXCEPTIONS
    : m_value(a_val)
{
}

template<typename T>
T AtomicValue<T>::operator++() NOEXCEPTIONS
{
    return __sync_add_and_fetch(&m_value ,1);
}

template<typename T>
T AtomicValue<T>::operator--() NOEXCEPTIONS
{
    return __sync_sub_and_fetch(&m_value ,1);
}

template<typename T>
void AtomicValue<T>::operator+=(T a_val) NOEXCEPTIONS
{
    __sync_add_and_fetch(&m_value ,a_val);
}

template<typename T>
void AtomicValue<T>::operator-=(T a_val) NOEXCEPTIONS
{
    return __sync_sub_and_fetch(&m_value ,a_val);
}

template<typename T>
T AtomicValue<T>::Value() const NOEXCEPTIONS
{
    return __sync_add_and_fetch(const_cast<T*>(&m_value) ,0);
}

template<typename T>
void AtomicValue<T>::Value(T a_val) NOEXCEPTIONS
{
    __sync_lock_test_and_set(&m_value, a_val);
}

class SoLibHandler 
{
public:
    SoLibHandler(const char* filename, int flags = RTLD_NOW);
    ~SoLibHandler();
    
    template <typename PF>
    PF getFunction(const char* name);

private:
    void* m_handle;
};

template <typename PF>
PF SoLibHandler::getFunction(const char* name)
{
    void* ps = dlsym(m_handle, name);
    if(ps == 0)
    {
        throw dlerror();
    }
    return reinterpret_cast<PF>(ps);
}

} //namespace advcpp

#endif //COMMON_UTILS_HPP