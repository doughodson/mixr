
#ifndef __mixr_base_ubf_Agent_HPP__
#define __mixr_base_ubf_Agent_HPP__

#include "mixr/base/IComponent.hpp"

namespace mixr {
namespace base {
namespace ubf {
class IBehavior;
class IState;
class IAction;

//------------------------------------------------------------------------------
// Class: Agent
// Description: Generic agent class to control a component in the simulation - the agent's "actor"
//              It manages a component (the "actor") with a behavior (either a player, or
//              a player's component)
//------------------------------------------------------------------------------
// Factory name: IAgent
//------------------------------------------------------------------------------
// Slots:
//    state       <IState>     ! The agent's state object
//    behavior    <IBehavior>  ! behavior
//------------------------------------------------------------------------------
// Notes:
// 1) Use 'Agent' to update the behavior framework via updateData() and use
//    'AgentTC' to update the behavior framework using updateTC().
//
// 2) The updateData() and updateTC() calls are only processed by this Agent
//    class and are not passed to the rest of the behavior framework.
//------------------------------------------------------------------------------
class Agent : public base::IComponent
{
   DECLARE_SUBCLASS(Agent, base::IComponent)

public:
   Agent();

   void updateData(const double dt = 0.0) override;
   void reset() override;

protected:
   // generic controller
   virtual void controller(const double dt = 0.0);

   IBehavior* getBehavior() const          { return behavior; }
   void setBehavior(IBehavior* const);

   IState* getState() const                { return state; }
   void setState(IState* const);

   virtual void initActor();

   base::IComponent* getActor();
   void setActor(base::IComponent* const myActor);

private:
   IBehavior* behavior{};
   IState* state{};
   safe_ptr<base::IComponent> myActor;

private:
   // slot table helper methods
   bool setSlotBehavior(IBehavior* const);
   bool setSlotState(IState* const);
};

inline void Agent::setActor(base::IComponent* const actor)      { myActor = actor; return; }
inline base::IComponent* Agent::getActor()                      { return myActor; }


//------------------------------------------------------------------------------
// Class: Agent
//
// Description: Generic agent class to control a component - the agent's "actor"
// - a derived agent class that performs its actions in the TC thread
//
// Factory name: UbfAgentTC
//------------------------------------------------------------------------------
class AgentTC : public Agent
{
   DECLARE_SUBCLASS(AgentTC, Agent)

public:
   AgentTC();

   void updateTC(const double dt = 0.0) override;
};

}
}
}

#endif

