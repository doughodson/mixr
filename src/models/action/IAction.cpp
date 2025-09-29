
#include "mixr/models/action/IAction.hpp"

#include "mixr/models/player/weapon/Bomb.hpp"
#include "mixr/models/player/IPlayer.hpp"
#include "mixr/models/system/OnboardComputer.hpp"
#include "mixr/models/system/Sar.hpp"
#include "mixr/models/system/IStoresMgr.hpp"
#include "mixr/models/navigation/Steerpoint.hpp"

#include "mixr/models/WorldModel.hpp"

#include "mixr/base/IComponent.hpp"
#include "mixr/base/Latitude.hpp"
#include "mixr/base/Longitude.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/qty/lengths.hpp"
#include "mixr/base/util/nav_utils.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IAction, "IAction")
EMPTY_SLOTTABLE(IAction)

IAction::IAction()
{
   STANDARD_CONSTRUCTOR()
}

void IAction::copyData(const IAction& org, const bool)
{
   BaseClass::copyData(org);

   refId = org.refId;
   completed = org.completed;

   setManager(nullptr);
}

void IAction::deleteData()
{
   setManager(nullptr);
}

//------------------------------------------------------------------------------
// Data access functions
//------------------------------------------------------------------------------

// True if action is ready to start
bool IAction::isReadyToStart()
{
   return !isInProgress() && !isCompleted();
}

// True if action is in progress
bool IAction::isInProgress()
{
   return (manager != nullptr);
}

// True if action has been completed
bool IAction::isCompleted()
{
   return completed;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the completed flag
void IAction::setCompleted(const bool flg)
{
   if (flg) setManager(nullptr);
   completed = flg;
}

// Sets the message ref ID
void IAction::setRefId(const int id)
{
   refId = id;
}

// Set our route manager
bool IAction::setManager(OnboardComputer* const mgr)
{
   manager = mgr;
   return true;
}

//------------------------------------------------------------------------------
// Starts this action
//------------------------------------------------------------------------------
bool IAction::trigger(OnboardComputer* const mgr)
{
   setManager(mgr);
   completed = false;
   return true;
}

//------------------------------------------------------------------------------
// Cancels this action
//------------------------------------------------------------------------------
bool IAction::cancel()
{
   completed = true;
   setManager(nullptr);
   return true;
}

//------------------------------------------------------------------------------
// process() -- action processing
//------------------------------------------------------------------------------
void IAction::process(const double)
{
}

//------------------------------------------------------------------------------
// Execute as an UBF action
// -- Use the 'actor' to find our OBC and 'trigger' this action.
//------------------------------------------------------------------------------
bool IAction::execute(base::IComponent* actor)
{
   bool ok{};
   if (actor != nullptr) {

      // Was the actor our OBC?
      auto obc = dynamic_cast<OnboardComputer*>( actor );

      // If not, was it our ownship ...
      //   and can we get our OBC from our ownship
      if (obc == nullptr) {
         const auto own = dynamic_cast<IPlayer*>( actor );
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
