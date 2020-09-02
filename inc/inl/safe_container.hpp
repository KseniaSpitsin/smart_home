#ifndef SAFE_CONTAINER_INL_HPP
#define SAFE_CONTAINER_INL_HPP

#include "safe_container.hpp"

template<class T, class ContainerT>
SafeContainer<T, ContainerT>::SafeContainer(size_t a_initialSize)
    : m_container(a_initialSize)
{
}

template<class T, class ContainerT>
void SafeContainer<T, ContainerT>::Push(const T& a_val)
{
    LockGuard guard(m_mutex);
    m_container.push_back(a_sharedPtr);
}

template<class T, class ContainerT>
void SafeContainer<T, ContainerT>::Pop()
{
    LockGuard guard(m_mutex);
    if(IsEmpty())
    {
        throw std::underflow_error("The container is empty");
    }

    m_container.pop_back();
}

template<class T, class ContainerT>
T& SafeContainer<T, ContainerT>::Back()
{
    LockGuard guard(m_mutex);

    if(IsEmpty())
    {
        throw std::underflow_error("The container is empty");
    }

    return m_container.back();
}

template<class T, class ContainerT>
T& SafeContainer<T, ContainerT>::operator[](size_t a_index)
{
    LockGuard guard(m_mutex);

    return m_container[a_index];
}

template<class T, class ContainerT>
size_t SafeContainer<T>::Size() const NOEXCEPTIONS
{
    LockGuard guard(m_mutex);

    return m_container.size();
}

template<class T, class ContainerT>
bool SafeContainer<T>::IsEmpty() const NOEXCEPTIONS
{
    LockGuard guard(m_mutex);

    return 0 == Size();
}

template<class T, class ContainerT>
void SafeContainer<T>::ClearAll() NOEXCEPTIONS
{
    LockGuard guard(m_mutex);

    m_container.clear();
}

template<class T, class ContainerT>
void SafeContainer<T>::Reserve(size_t a_newCapacity)
{
    LockGuard guard(m_mutex);

    m_container.reserve(a_newCapacity);
}

#endif //SAFE_CONTAINER_INL_HPP