#ifndef WAITABLE_BOUNDED_QUEUE_INL_HPP
#define WAITABLE_BOUNDED_QUEUE_INL_HPP

template<class ValT, class Container>
struct PredicateQueueNotFull
{
    PredicateQueueNotFull(const WaitableBoundedQueue<ValT, Container>& a_queueRef) NOEXCEPTIONS;
    bool operator()() const NOEXCEPTIONS;

    const WaitableBoundedQueue<ValT, Container>&  m_queueRef;
};

template<class ValT, class Container>
PredicateQueueNotFull<ValT, Container>::PredicateQueueNotFull(const WaitableBoundedQueue<ValT, Container>& a_queueRef) NOEXCEPTIONS
: m_queueRef(a_queueRef)
{
}

template<class ValT, class Container>
bool PredicateQueueNotFull<ValT, Container>::operator()() const NOEXCEPTIONS
{
    return m_queueRef.unsafeIsFull() && !m_queueRef.IsShutDown();
}

template<class ValT, class Container>
struct PredicateQueueNotEmpty
{
    PredicateQueueNotEmpty(const WaitableBoundedQueue<ValT, Container>& a_queueRef) NOEXCEPTIONS;
    bool operator()() const NOEXCEPTIONS;

    const WaitableBoundedQueue<ValT, Container>&  m_queueRef;
};

template<class ValT, class Container>
PredicateQueueNotEmpty<ValT, Container>::PredicateQueueNotEmpty(const WaitableBoundedQueue<ValT, Container>& a_queueRef) NOEXCEPTIONS
: m_queueRef(a_queueRef)
{
}

template<class ValT, class Container>
bool PredicateQueueNotEmpty<ValT, Container>::operator()() const NOEXCEPTIONS
{
    return m_queueRef.unsafeIsEmpty() && !m_queueRef.IsShutDown();
}

template<class ValT, class Container, class Predicate>
class WaitForEnqueue : private Uncopyable
{
public:
    WaitForEnqueue(const WaitableBoundedQueue<ValT, Container>& a_queueRef, Predicate a_predicator);
    //~WaitFor() = default;

private:
    LockGuard m_lock;
    CondVar m_enqueueCondVar;
};

template<class ValT, class Container, class Predicate>
WaitForEnqueue<ValT, Container, Predicate>::WaitForEnqueue
    (const WaitableBoundedQueue<ValT, Container>& a_queueRef, Predicate a_predicator)
    : Uncopyable()
    , m_lock(a_queueRef.unsafeGetMutex())
    , m_enqueueCondVar(a_queueRef.unsafeGetEnqueueCondVar())
{
    m_enqueueCondVar(m_lock, a_predicator);
}

template<class T, class FifoContainer>
WaitableBoundedQueue<T, FifoContainer>::WaitableBoundedQueue(size_t a_capacity)
    : Uncopyable()
    , m_container(a_capacity)
    , m_isShutDown(false)
    , m_mutex()
    , m_addedElement()
    , m_removedElement()
{
}

template<class T, class FifoContainer>
WaitableBoundedQueue<T, FifoContainer>::~WaitableBoundedQueue() NOEXCEPTIONS
{
    if(!m_isShutDown.Get())
    {
        turnOnShutDown();
    }
}

template<class T, class FifoContainer>
void WaitableBoundedQueue<T, FifoContainer>::Enqueue(const T& a_val)
{
    { //start of critical section
        LockGuard mutexGuard(m_mutex);
        m_removedElement.Wait(m_mutex, PredicateQueueNotFull<T, FifoContainer>(*this));
        
        if(m_isShutDown.Get())
        {
            handleShutDown();
        }

        try
        {
            bool result = m_container.Push(a_val);
            assert(result);
        }
        catch(...)
        {
            m_removedElement.NotifyOne();
            throw;
        }
    } //end of critical section

    m_addedElement.NotifyOne();
}


template<class T, class FifoContainer>
void WaitableBoundedQueue<T, FifoContainer>::Dequeue(T& a_val) 
{
    { //start of critical section
        LockGuard mutexGuard(m_mutex);
        m_addedElement.Wait(m_mutex, PredicateQueueNotEmpty<T, FifoContainer>(*this));
        if(m_isShutDown.Get()) 
        {
            handleShutDown();
        }

        try
        {
            bool result = m_container.Pop(a_val);
            assert(result);
        }
        catch(...) 
        {
            m_addedElement.NotifyOne();
            throw;
        }
    } //end of critical section

    m_removedElement.NotifyOne();
}

template<class T, class FifoContainer>
void WaitableBoundedQueue<T, FifoContainer>::ShutDown() NOEXCEPTIONS
{
    if(!m_isShutDown.Get())
    {
        turnOnShutDown();
    } 
}

template<class T, class FifoContainer>
bool WaitableBoundedQueue<T, FifoContainer>::unsafeIsFull() const NOEXCEPTIONS
{
    return m_container.IsFull();
}

template<class T, class FifoContainer>
bool WaitableBoundedQueue<T, FifoContainer>::unsafeIsEmpty() const NOEXCEPTIONS
{
    return m_container.IsEmpty();
}

template<class T, class FifoContainer>
bool WaitableBoundedQueue<T, FifoContainer>::IsShutDown() const NOEXCEPTIONS
{
    return m_isShutDown.Get();
}

template<class T, class FifoContainer>
Mutex& WaitableBoundedQueue<T, FifoContainer>::unsafeGetMutex() NOEXCEPTIONS
{
    return m_mutex;
}

template<class T, class FifoContainer>
CondVar& WaitableBoundedQueue<T, FifoContainer>::unsafeGetEnqueueCondVar() NOEXCEPTIONS
{
    return m_removedElement;
}

template<class T, class FifoContainer>
CondVar& WaitableBoundedQueue<T, FifoContainer>::unsafeGetDequeueCondVar() NOEXCEPTIONS
{
    return m_addedElement;
}

template<class T, class FifoContainer>
void WaitableBoundedQueue<T, FifoContainer>::turnOnShutDown() NOEXCEPTIONS 
{
    LockGuard mutexGuard(m_mutex);
    m_isShutDown.Acquire();
    m_addedElement.NotifyAll();
    m_removedElement.NotifyAll();
}

template<class T, class FifoContainer>
void WaitableBoundedQueue<T, FifoContainer>::handleShutDown()
{
    throw QueueIsShutingDown();
}

#endif //WAITABLE_BOUNDED_QUEUE_INL_HPP