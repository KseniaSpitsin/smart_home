#ifndef EVENT_PULLER_SHUT_DOWN_EXCEPTION_HPP
#define EVENT_PULLER_SHUT_DOWN_EXCEPTION_HPP

#include <exception>
#include "common_utils.hpp"

namespace smarthome
{

class EventPullerShutDownException : public std::exception
{
public:
    virtual const char* what() const NOEXCEPTIONS;
};

} //namespace smarthome

#endif //EVENT_PULLER_SHUT_DOWN_EXCEPTION_HPP