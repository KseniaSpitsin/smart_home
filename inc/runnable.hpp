#ifndef RUNNABLE_HPP
#define RUNNABLE_HPP

namespace advcpp {
    
class Runnable 
{
public:
    Runnable() throw();
    virtual ~Runnable() throw();
    virtual void Run() = 0;

private:
    Runnable(const Runnable& a_other);//copy not allowed
    Runnable& operator=(const Runnable& a_other);//assignment not allowed
};

} //advcpp

#endif //RUNNABLE_HPP