#ifndef POISONED_APPLE_EXCEPTION
#define POISONED_APPLE_EXCEPTION

#include <exception>

namespace advcpp 
{

class PoisonedAppleException : public std::exception 
{
private:
    friend class poisoned_apple;

    virtual const char* what() const throw() 
    {
        return "Oops! Poisoned apple!";
    }
};

} //advcpp


#endif //POISONED_APPLE_EXCEPTION