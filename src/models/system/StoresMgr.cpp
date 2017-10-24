
#include "mixr/models/system/StoresMgr.hpp"

#include "mixr/models/player/effect/Chaff.hpp"
#include "mixr/models/player/effect/Decoy.hpp"
#include "mixr/models/player/effect/Flare.hpp"
#include "mixr/models/player/weapon/Bomb.hpp"
#include "mixr/models/player/weapon/Missile.hpp"
#include "mixr/models/player/weapon/Sam.hpp"

#include "mixr/models/system/ExternalStore.hpp"
#include "mixr/models/system/FuelTank.hpp"
#include "mixr/models/system/Guns.hpp"
#include "mixr/models/system/OnboardComputer.hpp"
#include "mixr/models/system/RfSensor.hpp"
#include "mixr/models/system/trackmanager/TrackManager.hpp"
#include "mixr/models/Track.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include <cstring>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(StoresMgr, "BaseStoresMgr")
EMPTY_SLOTTABLE(StoresMgr)

BEGIN_EVENT_HANDLER(StoresMgr)
   ON_EVENT_OBJ(WPN_REL_EVENT,onWpnRelEvent,base::Boolean)
   ON_EVENT(WPN_REL_EVENT,onWpnRelEvent)

   ON_EVENT_OBJ(TRIGGER_SW_EVENT,onTriggerSwEvent,base::Boolean)
   ON_EVENT(TRIGGER_SW_EVENT,onTriggerSwEvent)

   ON_EVENT(WPN_RELOAD, onWpnReload)
END_EVENT_HANDLER()

StoresMgr::StoresMgr()
{
   STANDARD_CONSTRUCTOR()
}

void StoresMgr::copyData(const StoresMgr& org, const bool)
{
   BaseClass::copyData(org);

   gunFlg = org.gunFlg;
   mode = org.mode;
   masterArm = org.masterArm;
}

void StoresMgr::deleteData()
{
   setSlotStores(nullptr);
}

//------------------------------------------------------------------------------
// process() -- Process phase
//------------------------------------------------------------------------------
void StoresMgr::process(const double dt)
{
   BaseClass::process(dt);

   // Update the gun
   if (gunPtr != nullptr) {
      gunPtr->setGunArmed( isGunSelected() && isMasterArm(ARMED) );
      gunPtr->tcFrame(dt);
   }
}

//------------------------------------------------------------------------------
// shutdownNotification() -- We're shutting down
//------------------------------------------------------------------------------
bool StoresMgr::shutdownNotification()
{
   // Notify the external stores that we're shutting down
   base::PairStream* list{getStores()};
   if (list != nullptr) {
      base::List::Item* item{list->getFirstItem()};
      while (item != nullptr) {
         base::Pair* pair{static_cast<base::Pair*>(item->getValue())};
         base::Component* p{static_cast<base::Component*>(pair->object())};
         p->event(SHUTDOWN_EVENT);
         item = item->getNext();
      }
      list->unref();
      list = nullptr;
   }

   // Clear our stores
   setSlotStores(nullptr);

   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

// Is master arm mode == 'mode'
bool StoresMgr::isMasterArm(const unsigned int mode) const
{
   return (masterArm == mode);
}

// Returns the master arm mode
unsigned int StoresMgr::getMasterArm() const
{
   return masterArm;
}

// Is the delivery mode == 'testMode'
bool StoresMgr::isWeaponDeliveryMode(const unsigned int testMode) const
{
   return (mode == testMode);
}

// Returns the delivery mode
unsigned int StoresMgr::getWeaponDeliveryMode() const
{
   return mode;
}

// Pre-ref()'d list of our weapons
base::PairStream* StoresMgr::getWeapons()
{
   return weaponsList.getRefPtr();
}

// Pre-ref()'d list of our weapons (const version)
const base::PairStream* StoresMgr::getWeapons() const
{
   return weaponsList.getRefPtr();
}

// Pre-ref()'d list of our external equipment
base::PairStream* StoresMgr::getExternalStores()
{
   return externalList.getRefPtr();
}

// Pre-ref()'d list of our external equipment (const version)
const base::PairStream* StoresMgr::getExternalStores() const
{
   return externalList.getRefPtr();
}

// Pre-ref()'d list of our external fuel tanks
base::PairStream* StoresMgr::getExtFuelTanks()
{
   return fuelList.getRefPtr();
}

// Pre-ref()'d list of our external fuel tanks (const version)
const base::PairStream* StoresMgr::getExtFuelTanks() const
{
   return fuelList.getRefPtr();
}

// Get the gun model
Gun* StoresMgr::getGun()
{
   return gunPtr;
}

// Get the gun model (const version)
const Gun* StoresMgr::getGun() const
{
   return gunPtr;
}

// Are weapons are being released (default function)
bool StoresMgr::isWeaponReleased() const
{
   return false;
}

// Is the gun selected
bool StoresMgr::isGunSelected() const
{
   return gunFlg && (gunPtr != nullptr);
}

// Default function to get the current weapon (Pre-ref()'d)
AbstractWeapon* StoresMgr::getCurrentWeapon()
{
   // Get the selected station's weapon
   AbstractWeapon* wpn{getWeapon()};

   if (wpn == nullptr) {
      // If not found then check to see if the selected station
      // was really a Stores class object.  If so then ask it
      // for its selected station's weapon
      ExternalStore* es{getExternalStore()};
      if (es != nullptr) {
         const auto ss = dynamic_cast<Stores*>( es );
         if (ss != nullptr) wpn = ss->getWeapon();
         es->unref();
      }
   }

   return wpn;
}

const AbstractWeapon* StoresMgr::getCurrentWeapon() const
{
   // Get the selected station's weapon
   const AbstractWeapon* wpn{getWeapon()};

   if (wpn == nullptr) {
      // If not found then check to see if the selected station
      // was really a Stores class object.  If so then ask it
      // for its selected station's weapon
      const ExternalStore* es{getExternalStore()};
      if (es != nullptr) {
         const auto ss = dynamic_cast<const Stores*>( es );
         if (ss != nullptr) wpn = ss->getWeapon();
         es->unref();
      }
   }

   return wpn;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the master arming mode
bool StoresMgr::setMasterArm(const unsigned int newMode)
{
   masterArm = newMode;
   return true;
}

// Sets the weapon delivery mode
bool StoresMgr::setWeaponDeliveryMode(const unsigned int newMode)
{
   mode = newMode;
   return true;
}

// Selects the gun
bool StoresMgr::setGunSelected(const bool flg)
{
   gunFlg = (flg && gunPtr != nullptr);
   return true;
}

//------------------------------------------------------------------------------
// Empty "simple" get and release functions (derived classes can define these as needed)
//------------------------------------------------------------------------------

Missile* StoresMgr::getNextMissile()               { return nullptr; }
const Missile* StoresMgr::getNextMissile() const   { return nullptr; }
Sam* StoresMgr::getNextSam()                       { return nullptr; }
const Sam* StoresMgr::getNextSam() const           { return nullptr; }
Bomb* StoresMgr::getNextBomb()                     { return nullptr; }
const Bomb* StoresMgr::getNextBomb() const         { return nullptr; }
Chaff* StoresMgr::getNextChaff()                   { return nullptr; }
const Chaff* StoresMgr::getNextChaff() const       { return nullptr; }
Flare* StoresMgr::getNextFlare()                   { return nullptr; }
const Flare* StoresMgr::getNextFlare() const       { return nullptr; }
Decoy* StoresMgr::getNextDecoy()                   { return nullptr; }
const Decoy* StoresMgr::getNextDecoy() const       { return nullptr; }

Missile* StoresMgr::releaseOneMissile()   { return nullptr; }
Sam* StoresMgr::releaseOneSam()           { return nullptr; }
Bomb* StoresMgr::releaseOneBomb()         { return nullptr; }
Chaff* StoresMgr::releaseOneChaff()       { return nullptr; }
Flare* StoresMgr::releaseOneFlare()       { return nullptr; }
Decoy* StoresMgr::releaseOneDecoy()       { return nullptr; }

//------------------------------------------------------------------------------
// Empty event handlers (derived classes can define these as needed)
//------------------------------------------------------------------------------

// Default function to manage the weapon release event
bool StoresMgr::onWpnRelEvent(const base::Boolean* const)
{
   return true;
}

// Default function to manage the trigger switch event
bool StoresMgr::onTriggerSwEvent(const base::Boolean* const)
{
   return true;
}


//------------------------------------------------------------------------------
// Default weapons reload event handler
//------------------------------------------------------------------------------
bool StoresMgr::onWpnReload()
{
   // Reset the weapons only
   base::PairStream* list{getWeapons()};
   if (list != nullptr) {
      resetStores(list);
      list->unref();
      list = nullptr;
   }
   return true;
}

//------------------------------------------------------------------------------
// Search all of the objects in the main list for objects of 'type' and add
// them to the sublist.  Also check all Stores type objects for any 'type' objects.
//------------------------------------------------------------------------------
void StoresMgr::searchAndAdd(base::PairStream* const mainList, const std::type_info& type, base::PairStream* sublist)
{
   if (mainList != nullptr && sublist != nullptr) {

      const base::List::Item* item{mainList->getFirstItem()};
      while (item != nullptr) {

         const auto pair = static_cast<const base::Pair*>(item->getValue());
         const auto p = static_cast<const base::Component*>(pair->object());

         // Check the type and add to the list
         bool isType{p->isClassType(type)};
         if (isType) sublist->put(const_cast<base::Pair*>(pair));

         // If this is a Stores object then check its stores for 'type' objects as well
         const auto sp = dynamic_cast<const Stores*>(p);
         if ( sp != nullptr ) {
            const base::PairStream* pstores{sp->getStores()};
            if (pstores != nullptr) {
               searchAndAdd(const_cast<base::PairStream*>(pstores), type, sublist);
               pstores->unref();
            }
         }

         item = item->getNext();
      }
   }
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------
bool StoresMgr::setSlotStores(const base::PairStream* const msg)
{
   // First let our base class do everything that it needs to.
   BaseClass::setSlotStores(msg);

   // ---
   // Clear all previous stores and assigned weapons
   // ---
   weaponsList = nullptr;
   externalList = nullptr;
   fuelList = nullptr;
   gunPtr = nullptr;

   // ---
   // Use the stores list that the Stores class just processed.
   base::PairStream* stores{getStores()};
   if (stores != nullptr){

      // Create the new weapons list that contains all weapons
      {
         const auto newWeapons = new base::PairStream();
         searchAndAdd(stores, typeid(AbstractWeapon), newWeapons);
         if (newWeapons->entries() > 0) weaponsList = newWeapons;
         newWeapons->unref();
      }

      // Create the new external stores list that contains all
      // non-weapon, external stores (e.g., fuel tanks, pods, guns)
      {
         const auto newExternal = new base::PairStream();
         searchAndAdd(stores, typeid(ExternalStore), newExternal);
         if (newExternal->entries() > 0) externalList = newExternal;
         newExternal->unref();
      }

      // Create the new fuel tank list that contains all fuel tanks
      {
         const auto newFuel = new base::PairStream();
         searchAndAdd(stores, typeid(FuelTank), newFuel);
         if (newFuel->entries() > 0) fuelList = newFuel;
         newFuel->unref();
      }

      // Find the primary gun; i.e., the first gun found on our stores
      base::List::Item* item{stores->getFirstItem()};
      while (item != nullptr && gunPtr == nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto p = dynamic_cast<Gun*>(pair->object());
         if (p != nullptr) gunPtr = p;
         item = item->getNext();
      }

      stores->unref();
      stores = nullptr;
   }

   return true;
}

}
}
