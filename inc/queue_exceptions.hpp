#ifndef QUEUE_EXCEPTIONS_HPP
#define QUEUE_EXCEPTIONS_HPP

#include <exception>

namespace advcpp 
{

class QueueIsShutingDown : public std::exception 
{
    virtual const char* what() const throw() 
    {
        return "Queue is shuting down";
    }
};

} //advcpp

#endif //QUEUE_EXCEPTIONS_HPP