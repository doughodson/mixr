
#ifndef __mixr_models_SimAgent_H__
#define __mixr_models_SimAgent_H__

#include "mixr/base/ubf/Agent.hpp"

namespace mixr {
namespace simulation { class Station; }
namespace models {
class WorldModel;

//------------------------------------------------------------------------------
// Class: SimAgent
//
// Description: Generic agent class to control a component in the simulation - the agent's "actor"
//    newUbf version allows component behaviors to return null actions.
//    newUbf actions know how to execute themselves, so agent does not need to know anything about action class.
//    newUbf agent's state is initialized by slot, so agent does not need to know anything about state class.
//
// Description: An Agent that manages a component (the "actor") with a behavior
// (either a player, or a player's component)
//
// Factory name: SimAgent
// Slots:
//    actorPlayerName      <String>    ! The agent's actor - playerName
//    actorComponentName   <String>    ! The agent's actor - componentName
//------------------------------------------------------------------------------
class SimAgent : public base::ubf::Agent
{
   DECLARE_SUBCLASS(SimAgent, base::ubf::Agent)
public:
   SimAgent();

protected:

   void initActor() override;

   simulation::Station* getStation();
   WorldModel* getWorldModel();

   // sets which component ("actor") the agent is for
   void setActorPlayerByName(const char*);
   void setActorComponentByName(const char*);

private:
   const base::String* actorPlayerName {};
   const base::String* actorComponentName {};
   simulation::Station* myStation {};

private:
   // slot table helper methods
   // slot to set agent's actor (component to be controlled)
   bool setSlotActorPlayerName(const base::String* const);
   bool setSlotActorComponentName(const base::String* const);
};


// A SimAgent can be configured as shown below:  (assuming the existence of the "abc" state and behavior classes)
//( Station
//
//   components: {
//      simpleSimAgent: ( SimAgent
//         actorPlayerName: p01
//         state: ( abcState )
//         behavior: ( abcBehavior1 )
//      )
//   } // components
//
//   simExec: ( WorldModel
//
//      players: {
//
//         p01: ()
//
//         p02: ()
//      } // players
//   ) // simulation
//) // Station

}
}

#endif

