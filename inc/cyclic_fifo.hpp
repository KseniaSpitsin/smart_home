#ifndef CYCLIC_FIFO_HPP
#define CYCLIC_FIFO_HPP

#include <vector>
#include <cassert>

#include "common_utils.hpp"

namespace advcpp
{

//!@brief A class designed to operate as a container in a FIFO context (first-in first-out),
// where elements are inserted into one end of the container and extracted from the other.

template<class T>
//requirements for T:
    //Assignment operator
    //Copy Ctor
class CyclicFIFO
{
private:
    typedef std::vector<T> Container;

public:
    //!@param[in] a_maxCapacity - maximum size of the container
    //!@exception may throw exception of bad_alloc in case of failing in memory allocation
    CyclicFIFO(size_t a_maxCapacity);

    //~CyclicFIFO() = default;
    //CyclicFIFO(const CyclicFIFO& a_other) = default;
    //CyclicFIFO& operator=(const CyclicFIFO& a_other) = default;

    //!@brief inserts a new element at the end of the container.
    //!@param[in] a_value - The value to insert.
    //!@retval true if the inseration succeeded.
    // false if inseration failed because the container reached to it's maximum capacity.
    //!@exception may throw exception of assignment operator of T.
    bool Push(const T& a_value);

    //!@brief removes the element at the begin of the container and assigns
    // it's value to a_value.
    //!@retval true if the value removed successfully.
    // false if failed because there are no elements in the container.
    //!@exception may throw exception of assignment operator of T.
    bool Pop(T& a_value);

    //!@retval returns the number of elements in the container.
    //!@exception no-throw guarantee.
    size_t Size() const NOEXCEPTIONS;

    //!@retval true if the container's size is 0, false otherwise.
    //!@exception no-throw guarantee.
    bool IsFull() const NOEXCEPTIONS;

    //!@retval true if the container's size equals to it's max capacity, false otherwise.
    //!@exception no-throw guarantee.
    bool IsEmpty() const NOEXCEPTIONS;

private:
    size_t calcNextIndexPlacement(size_t a_currentIndexPlace) const NOEXCEPTIONS;

    void increaseNumOfElements() NOEXCEPTIONS;
    void decreaseNumOfElements() NOEXCEPTIONS;

private:
    Container m_container;

    size_t m_capacity;
    size_t m_readerIndex;
    size_t m_writerIndex;
    size_t m_currNumOfElem;
};

#include "inl/cyclic_fifo.hpp"

} //advcpp

#endif //CYCLIC_FIFO_HPP