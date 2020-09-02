#ifndef WAITABLE_BOUNDED_HPP
#define WAITABLE_BOUNDED_HPP

#include "common_utils.hpp"
#include "cond_var.hpp"
#include "mutex.hpp"
#include "queue_exceptions.hpp"
#include "cyclic_fifo.hpp"

namespace advcpp
{

//!@brief A thread safe queue - data can safely be added by one thread and removed by another thread
// without corrupting the data structure

template<class T, class FifoContainer = CyclicFIFO<T> >
class WaitableBoundedQueue : private Uncopyable
//requirements for T:
    //Assignment operator
    //Copy Ctor
//requirements for FifoContainer:
    //Ctor(size_t a_capacity)
    //bool Push(const T&)
    //bool Pop(T&)
    //bool IsEmpty() const
    //bool IsFull() const
{
public:
    //!@param[in] a_capacityCreates a queue with maximum a_capcity.
    WaitableBoundedQueue(size_t a_capacity);
    ~WaitableBoundedQueue() NOEXCEPTIONS;

    //!@brief inserts a_val to the queue in a FIFO context.
    // if the queue is full the thread will block until an element is removed and the 
    // value can be inserted.
    //!@param[in] a_val - the value to insert.
    //!@exception throws exception QueueIsShutingDown() when the queue
    // starts in process of shutdown.
    // may throw exception of assignment operator of T.
    void Enqueue(const T& a_val);

    //!@brief removes an element from the queue in a FIFO context(the oldest value will be removed),
    // and assigns it to a_val.
    // if the queue is empty the thread will block until an element is inserted.
    //!@param[in] a_val - the value to insert to.
    //!@exception throws exception QueueIsShutingDown() when the queue
    // starts in process of shutdown.
    // may throw exception of assignment operator of T.
    void Dequeue(T& a_val);

    //!@brief shuts down the working queue.
    void ShutDown() NOEXCEPTIONS;

private:
    template<class ValT, class Container>
    friend class PredicateQueueNotFull;

    template<class ValT, class Container>
    friend class PredicateQueueNotEmpty;

    template<class ValT, class Container, class Predicate>
    friend class WaitForEnqueue;

private:
    //!@retval true if the queue is empty, false otherwise.
    //!This function is not thread safe!
    bool unsafeIsEmpty() const NOEXCEPTIONS;

    //!@retval true if the queue is full, false otherwise.
    //!This function is not thread safe!
    bool unsafeIsFull() const NOEXCEPTIONS;

    //!@retval true if the queue is shuting down, false otherwise.
    bool IsShutDown() const NOEXCEPTIONS;

    //!@retval reference to the mutex.
    //!This function is not thread safe!
    Mutex& unsafeGetMutex() NOEXCEPTIONS;

    //!@retval reference to the cond var used in Enqueue.
    //!This function is not thread safe!
    CondVar& unsafeGetEnqueueCondVar() NOEXCEPTIONS;

    //!@retval reference to the cond var used in Dequeue.
    //!This function is not thread safe!
    CondVar& unsafeGetDequeueCondVar() NOEXCEPTIONS;

private:
    void turnOnShutDown() NOEXCEPTIONS;
    void handleShutDown();

private:
    FifoContainer m_container;

    AtomicFlag m_isShutDown;

    Mutex m_mutex;
    CondVar m_addedElement;
    CondVar m_removedElement;
};

#include "inl/waitable_bounded_queue.hpp"

} //advcpp

#endif //WAITABLE_BOUNDED_HPP