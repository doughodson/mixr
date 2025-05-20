
#include "mixr/models/action/Action.hpp"

#include "mixr/models/player/weapon/Bomb.hpp"
#include "mixr/models/player/Player.hpp"
#include "mixr/models/system/OnboardComputer.hpp"
#include "mixr/models/system/Sar.hpp"
#include "mixr/models/system/StoresMgr.hpp"
#include "mixr/models/navigation/Steerpoint.hpp"

#include "mixr/models/WorldModel.hpp"

#include "mixr/base/IComponent.hpp"
#include "mixr/base/Latitude.hpp"
#include "mixr/base/Longitude.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/units/lengths.hpp"
#include "mixr/base/util/nav_utils.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Action, "Action")
EMPTY_SLOTTABLE(Action)

Action::Action()
{
   STANDARD_CONSTRUCTOR()
}

void Action::copyData(const Action& org, const bool)
{
   BaseClass::copyData(org);

   refId = org.refId;
   completed = org.completed;

   setManager(nullptr);
}

void Action::deleteData()
{
   setManager(nullptr);
}

//------------------------------------------------------------------------------
// Data access functions
//------------------------------------------------------------------------------

// True if action is ready to start
bool Action::isReadyToStart()
{
   return !isInProgress() && !isCompleted();
}

// True if action is in progress
bool Action::isInProgress()
{
   return (manager != nullptr);
}

// True if action has been completed
bool Action::isCompleted()
{
   return completed;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the completed flag
void Action::setCompleted(const bool flg)
{
   if (flg) setManager(nullptr);
   completed = flg;
}

// Sets the message ref ID
void Action::setRefId(const int id)
{
   refId = id;
}

// Set our route manager
bool Action::setManager(OnboardComputer* const mgr)
{
   manager = mgr;
   return true;
}

//------------------------------------------------------------------------------
// Starts this action
//------------------------------------------------------------------------------
bool Action::trigger(OnboardComputer* const mgr)
{
   setManager(mgr);
   completed = false;
   return true;
}

//------------------------------------------------------------------------------
// Cancels this action
//------------------------------------------------------------------------------
bool Action::cancel()
{
   completed = true;
   setManager(nullptr);
   return true;
}

//------------------------------------------------------------------------------
// process() -- action processing
//------------------------------------------------------------------------------
void Action::process(const double)
{
}

//------------------------------------------------------------------------------
// Execute as an UBF action
// -- Use the 'actor' to find our OBC and 'trigger' this action.
//------------------------------------------------------------------------------
bool Action::execute(base::IComponent* actor)
{
   bool ok{};
   if (actor != nullptr) {

      // Was the actor our OBC?
      auto obc = dynamic_cast<OnboardComputer*>( actor );

      // If not, was it our ownship ...
      //   and can we get our OBC from our ownship
      if (obc == nullptr) {
         const auto own = dynamic_cast<Player*>( actor );
         if (own != nullptr) {
            obc = own->getOnboardComputer();
         }
      }

      // If we could find our OBC from the actor then trigger this action
      if (obc != nullptr) {
         ok = trigger(obc);
      }

   }
   return ok;
}

}
}
