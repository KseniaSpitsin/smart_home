#ifndef SAFE_CONTAINER_HPP
#define SAFE_CONTAINER_HPP

#include <vector>
#include <stdexcept>

#include "common_utils.hpp"
#include "shared_ptr.hpp"
#include "mutex.hpp"

namespace advcpp
{

template<class T, class ContainerT = std::vector<T> >
//req for T:
    //copy ctor, operator=
//req for ContainerT:
    //push_back
    //pop_back
    //clear
    //size
    //operator[]
    //reserve
class SafeContainer
{
private:
    typedef std::vector<T> Container;

public:
    explicit SafeContainer(size_t a_initialSize = 0);

    //!@brief Adds a_sharedPtr to the back of the container.
    void Push(const T& a_val);
    
    //!@brief Removes an element from the back of the container.
    //!@exception throws std::underflow_error if the container is empty
    void Pop();

    //!@brief returns reference to the elment at the end of the container T
    //!@exception throws std::underflow_error if the container is empty
    T& Back();
    
    //!@brief returns shared ptr of T
    T& operator[](size_t a_index);

    //!@retval the number of elements in the container.
    //!@exception no-throw guarantee
    size_t Size() const NOEXCEPTIONS;

    //!@retval true if the container is empty, false otherwise.
    //!@exception no-throw guarantee
    bool IsEmpty() const NOEXCEPTIONS;

    //!@brief removes all elements from the container (which are destroyed),
    // leaving the container with a size of 0.
    void ClearAll() NOEXCEPTIONS;
    
    //!@brief Increase the capacity of the container to a value that's greater or equal to
    // a_newCapcity.
    void Reserve(size_t a_newCapacity);

private:
    bool isInRange(size_t a_index) const NOEXCEPTIONS;

private:
    Container m_container;
    Mutex a_mutex;

};

#include "inl/safe_container.hpp"

} //advcpp

#endif //SAFE_CONTAINER_HPP