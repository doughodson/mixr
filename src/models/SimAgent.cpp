
#include "mixr/models/SimAgent.hpp"

#include "mixr/models/player/Player.hpp"
#include "mixr/models/WorldModel.hpp"

#include "mixr/simulation/Station.hpp"

#include "mixr/base/Pair.hpp"
#include "mixr/base/String.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(SimAgent, "SimAgent")
EMPTY_COPYDATA(SimAgent)

BEGIN_SLOTTABLE(SimAgent)
   "actorPlayerName",                 //  1) The agent's actor - playerName
   "actorComponentName"               //  1) The agent's actor - componentName
END_SLOTTABLE(SimAgent)

BEGIN_SLOT_MAP(SimAgent)
   ON_SLOT(1, setSlotActorPlayerName,    base::String )
   ON_SLOT(2, setSlotActorComponentName, base::String )
END_SLOT_MAP()

SimAgent::SimAgent()
{
   STANDARD_CONSTRUCTOR()
}

void SimAgent::deleteData()
{
   if (actorPlayerName!=nullptr) {
      actorPlayerName->unref();
      actorPlayerName=nullptr;
   }
   if (actorComponentName!=nullptr) {
      actorComponentName->unref();
      actorComponentName=nullptr;
   }
}

simulation::Station* SimAgent::getStation()
{
   if ( myStation==nullptr ) {
      const auto s = dynamic_cast<simulation::Station*>(findContainerByType(typeid(simulation::Station)));
      if (s != nullptr) {
         myStation = s;
      }
   }
   return myStation;
}

WorldModel* SimAgent::getWorldModel()
{
   WorldModel* sim{};
   simulation::Station* s{getStation()};
   if (s != nullptr) {
      sim = dynamic_cast<WorldModel*>(s->getSimulation());
   }
   return sim;
}

// finds our actor during reset() processing
void SimAgent::initActor()
{
   if (getActor() == nullptr ) {
      if (actorPlayerName == nullptr) {
         // not correctly specified as a SimAgent, try baseClass ?
         BaseClass::initActor();
      } else {
         WorldModel* sim{getWorldModel()};
         if ( sim != nullptr ) {
            base::Component* player{sim->findPlayerByName(actorPlayerName->c_str())};
            if (actorComponentName == nullptr) {
               // no player component specified, so the player is the actor
               setActor(player);
            } else if (player != nullptr) {
               base::Pair* pair{player->findByName(actorComponentName->c_str())};
               if (pair != nullptr) {
                  setActor(dynamic_cast<base::Component*>( pair->object() ));
               }
            }
         }
      }
   }
}

void SimAgent::setActorPlayerByName(const char* x)
{
   actorPlayerName = new base::String(x);
}
void SimAgent::setActorComponentByName(const char* x)
{
   actorComponentName = new base::String(x);
}
//------------------------------------------------------------------------------
// set slot functions
//------------------------------------------------------------------------------

bool SimAgent::setSlotActorPlayerName(const base::String* const x)
{
   bool ok{};
   if ( x != nullptr ) {
      setActorPlayerByName(x->c_str());
      ok = true;
   }
   return ok;
}

bool SimAgent::setSlotActorComponentName(const base::String* const x)
{
   bool ok{};
   if ( x != nullptr ) {
      setActorComponentByName(x->c_str());
      ok = true;
   }
   return ok;
}

}
}


