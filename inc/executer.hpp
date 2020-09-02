#ifndef EXECUTER_HPP
#define EXECUTER_HPP

#include "common_utils.hpp"
#include "run_executer.hpp"
#include "r_thread.hpp"

namespace advcpp
{

namespace thread_group_impl
{
    class Executer
    {
        public:
            Executer(RunExecuter* a_runnable);
            //~Executer() = default;
            //Executer(const Executer& a_other) = default;
            //Executer& operator=(const Executer& a_other) = default;

            void Join();

            bool TryTimedJoin(size_t a_microSec);

            bool IsRunning() NOEXCEPTIONS;

        private:
            SharedPtr<RunExecuter> m_runnable;
            SharedPtr<RThread> m_thread; //has to be after m_runnable
    };

} //namespace thread_group_impl

} //namespace advcpp

#endif //EXECUTER_HPP