#ifndef COND_VAR_EXCEPTIONS_HPP
#define COND_VAR_EXCEPTIONS_HPP

#include <exception>
#include "common_utils.hpp"

namespace advcpp
{

class CondVarLackOfSources : public std::exception
{
public:
    virtual const char* what() const NOEXCEPTIONS
    { 
        return "Lack of necessary resources to initialize the condition variable";
    }
};

class CondVarLackOfMemory : public std::exception
{
public:
    virtual const char* what() const NOEXCEPTIONS
    { 
        return "Lack of memory to initialize the condition variable";
    }
};

class CondVarOwnershipError : public std::exception
{
public:
	virtual const char* what() const NOEXCEPTIONS
    { 
        return "The mutex was not owned the time of the call";
    }
};


} //advcpp

#endif //COND_VAR_EXCEPTIONS_HPP