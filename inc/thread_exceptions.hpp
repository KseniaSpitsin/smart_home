#ifndef THREAD_EXCEPTIONS
#define THREAD_EXCEPTIONS

#include <exception>

namespace advcpp
{

class ThreadLackOfResource : public std::exception 
{
public:
    virtual const char* what() const NOEXCEPTIONS
    { 
        return "Insufficient resources to create a thread or limit on the number of threads was encountered";
    }
};

class ThreadDeadlock : public std::exception 
{
public:
    virtual const char* what() const NOEXCEPTIONS
    { 
        return "A deadlock was detected (e.g., two threads tried to join with each other)";
    }
};

} //advcpp

#endif //THREAD_EXCEPTIONS