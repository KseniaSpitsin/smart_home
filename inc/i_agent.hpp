#ifndef I_AGENT_HPP
#define I_AGENT_HPP

#include "common_utils.hpp"

namespace smarthome
{

class IRegistrar;
class IEventPusher;

class IAgent : private advcpp::Uncopyable
{
public:
    //IAgent() = default;
    virtual ~IAgent();

    virtual void Connect(const SharedPtr<IRegistrar>& a_registrar, const SharedPtr<IEventPusher>& a_pusher) = 0;
    virtual void Disconnect() = 0;
};

} //namespace smarthome

#endif //I_AGENT_HPP