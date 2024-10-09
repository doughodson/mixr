
#include "mixr/models/system/IrSystem.hpp"

#include "mixr/models/player/Player.hpp"
#include "mixr/models/system/IrSeeker.hpp"
#include "mixr/models/system/OnboardComputer.hpp"
#include "mixr/models/Tdb.hpp"

#include "mixr/models/WorldModel.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/PairStream.hpp"

#include <string>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IrSystem, "IrSystem")

BEGIN_SLOTTABLE(IrSystem)
   "seekerName",        //  1: Name of the requested Seeker  (base::String)
   "disableQueries",    //  2: Disable sending queries packets flag (default: false)
END_SLOTTABLE(IrSystem)

BEGIN_SLOT_MAP(IrSystem)
   ON_SLOT(1,  setSlotSeekerName,      base::Identifier)
   ON_SLOT(2,  setSlotDisableQueries,  base::Boolean)
END_SLOT_MAP()

IrSystem::IrSystem()
{
   STANDARD_CONSTRUCTOR()
}

void IrSystem::copyData(const IrSystem& org, const bool)
{
   BaseClass::copyData(org);

   disableQueries = org.disableQueries;
   seekerName = org.seekerName;
}

void IrSystem::deleteData()
{
   setSeeker(nullptr);
}

//------------------------------------------------------------------------------
// shutdownNotification()
//------------------------------------------------------------------------------
bool IrSystem::shutdownNotification()
{
   setSeeker(nullptr);
   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void IrSystem::reset()
{
   // FAB - sensor needs to know its seeker without waiting for updateData
   if (getSeeker() == nullptr && (seekerName != "") && getOwnship() != nullptr) {
      // We have a name of the seeker, but not the seeker itself
      // Get the named seeker from the player's list of gimbals
      const auto p = dynamic_cast<IrSeeker*>( getOwnship()->getGimbalByName(seekerName.c_str()) );
      if (p != nullptr) {
         setSeeker( p );
      }

      if (getSeeker() == nullptr) {
         // The assigned seeker was not found!
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "IrSystem::update() ERROR -- seeker: " << seekerName << ", was not found!" << std::endl;
         }
         setSlotSeekerName(nullptr);
      }
   }

   BaseClass::reset();
}

//------------------------------------------------------------------------------
// updateData() -- update background data here
//------------------------------------------------------------------------------
void IrSystem::updateData(const double dt)
{
   // ---
   // Do we need to find the seeker?
   // ---
   if (getSeeker() == nullptr && (seekerName != "") && getOwnship() != nullptr) {
      // We have a name of the seeker, but not the seeker itself
      // Get the named seeker from the player's list of gimbals
      const auto p = dynamic_cast<IrSeeker*>( getOwnship()->getGimbalByName(seekerName.c_str()) );
      if (p != nullptr) {
         setSeeker( p );
         // FAB - not needed - esp if multiple sensors on a seeker.
         //getSeeker()->setSystem(this);
      }

      if (getSeeker() == nullptr) {
         // The assigned seeker was not found!
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "IrSystem::update() ERROR -- seeker: " << seekerName << ", was not found!" << std::endl;
         }
         setSlotSeekerName(nullptr);
      }
   }

   // ---
   // Process our players of interest
   // ---
   processPlayersOfInterest();

   // ---
   // Our base class methods
   // ---
   BaseClass::updateData(dt);
}

//------------------------------------------------------------------------------
// Process players of interest -- This will work with the function in Gimbal to create
// a filtered list of players that we plan to send emission packets to.
//------------------------------------------------------------------------------
void IrSystem::processPlayersOfInterest()
{
   // ---
   // Do we have a seeker?
   // ---
   if (getSeeker() != nullptr) {
      base::PairStream* poi{};
      WorldModel* sim{getWorldModel()};
      if ( sim != nullptr && !areQueriesDisabled() )
         poi = getWorldModel()->getPlayers();

      // Pass our players of interest to the seeker for processing
      getSeeker()->processPlayersOfInterest(poi);

      if (poi != nullptr) { poi->unref(); poi = nullptr; }
   }
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

// isQuerying() -- Returns true if the IR system is currently observing targets
bool IrSystem::isQuerying() const
{
    // Default: if we're enabled and have a seeker, we're transmitting.
    return ( !areQueriesDisabled() && getSeeker() != nullptr && getOwnship() != nullptr );
}

// Returns true if sending query packets has been disabled
bool IrSystem::areQueriesDisabled() const
{
   return disableQueries;
}

// Pointer to the seeker model, or zero (0) if none
IrSeeker* IrSystem::getSeeker()
{
   return seeker;
}

// Pointer to the seeker model, or zero (0) if none
const IrSeeker* IrSystem::getSeeker() const
{
   return seeker;
}

// Name of the seeker model, or zero (0) if none
const std::string& IrSystem::getSeekerName() const
{
   return seekerName;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Disables/enables sending the R/F emissions packets
bool IrSystem::setDisableQueriesFlag(const bool b)
{
   disableQueries = b;
   return true;
}

// setSeeker() -- set the seeker
bool IrSystem::setSeeker(IrSeeker* const p)
{
   if (seeker != nullptr) {
      seeker->unref();
   }
   seeker = p;
   if (seeker != nullptr) {
      seeker->ref();
   }
   return true;
}

//------------------------------------------------------------------------------
// Slot Functions  (return 'true' if the slot was set, else 'false')
//------------------------------------------------------------------------------

// seekerName: IrSeeker name  (base::String)
bool IrSystem::setSlotSeekerName(base::Identifier* const p)
{
   if (p == nullptr) return false;
   seekerName = p->asString();
   return true;
}

// setSlotDisableQueries() -- sets the disable sending emissions flag
bool IrSystem::setSlotDisableQueries(base::Boolean* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setDisableQueriesFlag( msg->asBool() );
   }
   return ok;
}

}
}
