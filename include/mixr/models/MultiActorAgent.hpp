
#ifndef __mixr_models_common_MultiActorAgent_HPP__
#define __mixr_models_common_MultiActorAgent_HPP__

#include "mixr/base/IComponent.hpp"
#include "mixr/base/Identifier.hpp"

#include <array>
#include <string>

namespace mixr {
namespace base {
class Identifier;
namespace ubf { class IBehavior; class IState; }
}
namespace simulation { class IStation; }
namespace models {
class WorldModel;

//------------------------------------------------------------------------------
// Class: MultiActorAgent
//
// Description: Generic agent class to control a list of actors, each specified with their own behavior
//    the only reason to use this class is if there is state shared between multiple actors
//    (if there is not shared state, just use a list of standard Agent instances)
//
// This class assumes that state has two elements - shared state for all actors, and
// local state for each actor
//
// Factory name: MultiActorAgent
// Slots:
//    state       <State>           ! state
//    agentList   <IPairStream>     ! behavior pairstream
//------------------------------------------------------------------------------
class MultiActorAgent : public base::IComponent
{
   DECLARE_SUBCLASS(MultiActorAgent, base::IComponent)

public:
   MultiActorAgent();

   void updateData(const double dt = 0.0) override;
   void reset() override;

protected:
   // generic controller
   virtual void controller(const double dt = 0.0);

   void setState(base::ubf::IState* const);
   base::ubf::IState* getState() const                { return state; }

   void setActor(base::IComponent* c);
   base::IComponent* getActor()                        { return actor;}

   simulation::IStation* getStation();
   WorldModel* getWorldModel();

   struct AgentItem
   {
      std::string actorName;
      base::safe_ptr<base::ubf::IBehavior> behavior;
      base::safe_ptr<base::IComponent> actor;
   };

   static const int MAX_AGENTS{10};
   bool clearAgentList();
   bool addAgent(const std::string& name, base::ubf::IBehavior* const);

private:
   base::IComponent* actor{};
   base::ubf::IState* state{};
   simulation::IStation* myStation{};

   // agent/behavior list
   unsigned int nAgents{};          // Number of input behavior/agent pairs
   std::array<AgentItem, MAX_AGENTS> agentList;

private:
   // slot table helper methods
   bool setSlotState(base::ubf::IState* const);
   bool setSlotAgentList(base::IPairStream* const);
};

inline void MultiActorAgent::setActor(base::IComponent* c) { actor=c; }

}
}


// A MultiActorAgent can be configured as shown below: (assuming the existence of the "abc" state and behavior classes)
//( Station
//   components: {
//      agent: ( MultiActorAgent
//         state: ( abcState )
//         agentList: {
//            // use slotname to specify the player name (the "actor") to which to connect the agent/behavior
//            p1: ( abcBehavior1 )
//            p2: ( abcBehavior2 )
//         }
//      )
//   } // components
//
//   simulation: ( WorldModel
//
//      players: {
//
//         p1: ()
//
//         p2: ()
//
//      } // players
//   ) // simulation
//) // Station

#endif

