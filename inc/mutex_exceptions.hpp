#ifndef MUTEX_EXCEPTIONS_HPP
#define MUTEX_EXCEPTIONS_HPP

#include <exception>
#include "common_utils.hpp"

namespace advcpp 
{

class MutexLackOfSource : public std::exception 
{
public:
    virtual const char* what() const NOEXCEPTIONS
    { 
        return "The system lacked the necessary resources to initialize the mutex";
    }
};

class MutexLackOfMemory : public std::exception 
{
public:
    virtual const char* what() const NOEXCEPTIONS
    { 
        return "Insufficient memory to initialize the mutex";
    }
};

class MutexOwnershipError : public std::exception
{
public:
    virtual const char* what() const NOEXCEPTIONS
    { 
        return "The current thread does not own the mutex";
    }
};

class MutexLockAgainError : public std::exception
{
public:
    virtual const char* what() const NOEXCEPTIONS
    { 
        return "The current thread already owns the mutex";
    } 
};

class MutexMaxRecursive : public std::exception
{
public:
    virtual const char* what() const NOEXCEPTIONS
    {
        return "The maximum number of recursive locks for mutex has been exceeded.";
    }
};

}//advcpp

#endif//__MUTEX_EXCEPTIONS__