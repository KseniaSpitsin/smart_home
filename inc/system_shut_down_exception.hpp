#ifndef SYSTEM_SHUT_DOWN_EXCEPTION_HPP
#define SYSTEM_SHUT_DOWN_EXCEPTION_HPP

#include <exception>
#include "common_utils.hpp"

namespace smarthome
{

class SystemShutDownException : public std::exception
{
    friend class SystemShutDownHandler;

public:
    virtual const char* what() const NOEXCEPTIONS;

private:
    SystemShutDownException();
};

} //namespace smarthome

#endif //SYSTEM_SHUT_DOWN_EXCEPTION_HPP