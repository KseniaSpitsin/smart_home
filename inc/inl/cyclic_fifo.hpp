#ifndef CYCLIC_FIFO_INL_HPP
#define CYCLIC_FIFO_INL_HPP

template<class T>
CyclicFIFO<T>::CyclicFIFO(size_t a_maxCapacity)
    : m_container(a_maxCapacity)
    , m_capacity(a_maxCapacity)
    , m_readerIndex(0)
    , m_writerIndex(0)
    , m_currNumOfElem(0)
{
}

template<class T>
bool CyclicFIFO<T>::Push(const T& a_value)
{
    if(IsFull())
    {
        return false;
    }

    m_container[m_writerIndex] = a_value;
    m_writerIndex = calcNextIndexPlacement(m_writerIndex);
    increaseNumOfElements();

    return true;
}

template<class T>
bool CyclicFIFO<T>::Pop(T& a_value)
{
    if(IsEmpty())
    {
        return false;
    }

    a_value = m_container[m_readerIndex];
    m_readerIndex = calcNextIndexPlacement(m_readerIndex);
    decreaseNumOfElements();

    return true;
}

template<class T>
size_t CyclicFIFO<T>::Size() const NOEXCEPTIONS
{
    return m_currNumOfElem;
}

template<class T>
bool CyclicFIFO<T>::IsFull() const NOEXCEPTIONS
{
    return m_currNumOfElem == m_capacity;
}

template<class T>
bool CyclicFIFO<T>::IsEmpty() const NOEXCEPTIONS
{
    return 0 == m_currNumOfElem;
}

template<class T>
size_t CyclicFIFO<T>::calcNextIndexPlacement(size_t a_currentIndexPlace) const NOEXCEPTIONS
{
    return (a_currentIndexPlace + 1) % m_capacity;
}

template<class T>
void CyclicFIFO<T>::increaseNumOfElements() NOEXCEPTIONS
{
    assert(!IsFull());
    ++m_currNumOfElem;
}

template<class T>
void CyclicFIFO<T>::decreaseNumOfElements() NOEXCEPTIONS
{
    assert(!IsEmpty());
    --m_currNumOfElem;
}

#endif //CYCLIC_FIFO_INL_HPP