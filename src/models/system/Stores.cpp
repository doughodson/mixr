
#include "mixr/models/system/Stores.hpp"
#include "mixr/models/player/weapon/IWeapon.hpp"

#include "mixr/base/String.hpp"
#include "mixr/base/Identifier.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/util/string_utils.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Stores, "Stores")

BEGIN_SLOTTABLE(Stores)
   "numStations",  //  1: Number of stations (less than or equal MAX_STATIONS)
   "stores",      //  2: Our weapons and other external stores
   "selected"     //  3: Selected weapon station number
END_SLOTTABLE(Stores)

BEGIN_SLOT_MAP(Stores)
   ON_SLOT( 1, setSlotNumStations, base::Integer)
   ON_SLOT( 2, setSlotStores,      base::PairStream)
   ON_SLOT( 3, setSlotSelected,    base::Integer)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(Stores)
    ON_EVENT_OBJ( JETTISON_EVENT, onJettisonEvent, IWeapon )
    ON_EVENT_OBJ( JETTISON_EVENT, onJettisonEvent, IExternalStore )
END_EVENT_HANDLER()

Stores::Stores()
{
   STANDARD_CONSTRUCTOR()
}

void Stores::copyData(const Stores& org, const bool)
{
   IObject::copyData(org);

   ns = org.ns;
   selected = org.selected;
   setSlotStores(org.storesList); // setSlotStores() clones the stores
}

void Stores::deleteData()
{
   setSlotStores(nullptr);
   setNumberOfStations(0);
}

//------------------------------------------------------------------------------
// Reset()
//------------------------------------------------------------------------------
void Stores::reset()
{
   BaseClass::reset();

   // Reset all of the stores
   base::PairStream* stores{getStores()};
   if (stores != nullptr) {
      resetStores(stores);
      stores->unref();
      stores = nullptr;
   }
}

//------------------------------------------------------------------------------
// process() -- Process phase
//------------------------------------------------------------------------------
void Stores::process(const double dt)
{
   // check our blocked flags
   updateBlockedFlags();

   BaseClass::process(dt);
}

//------------------------------------------------------------------------------
// updateTC() -- update time critical stuff here
//------------------------------------------------------------------------------
void Stores::updateTC(const double dt)
{
   // Update our non-weapon, external stores, which need to act as
   // active systems attached to our ownship player.
   {
      base::PairStream* list{getStores()};
      if (list != nullptr) {
         base::List::Item* item{list->getFirstItem()};
         while (item != nullptr) {
            const auto pair = static_cast<base::Pair*>(item->getValue());
            const auto p = dynamic_cast<IExternalStore*>( pair->object() );
            if (p != nullptr) p->updateTC(dt);
            item = item->getNext();
         }
         list->unref();
         list = nullptr;
      }
   }

   BaseClass::updateTC(dt);
}

//------------------------------------------------------------------------------
// updateData() -- update non-time critical stuff here
//------------------------------------------------------------------------------
void Stores::updateData(const double dt)
{
   // Update our non-weapon, external stores, which need to act as
   // active systems attached to our ownship player.
   {
      base::PairStream* list{getStores()};
      if (list != nullptr) {
         base::List::Item* item{list->getFirstItem()};
         while (item != nullptr) {
            const auto pair = static_cast<base::Pair*>(item->getValue());
            const auto p = dynamic_cast<IExternalStore*>( pair->object() );
            if (p != nullptr) p->updateData(dt);
            item = item->getNext();
         }
         list->unref();
         list = nullptr;
      }
   }

   BaseClass::updateData(dt);
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

// Number of stations on the launcher
unsigned int Stores::getNumberOfStations() const
{
   return ns;
}

// Returns the list of our external stores
base::PairStream* Stores::getStores()
{
   return storesList.getRefPtr();
}

// Returns the list of our external stores (const version)
const base::PairStream* Stores::getStores() const
{
   return storesList.getRefPtr();
}

// Number of weapons on the launcher
unsigned int Stores::getNumberOfWeapons() const
{
   return numWpn;
}

// Returns the number of weapons available for launch
unsigned int Stores::available() const
{
   unsigned int n{};
   for (unsigned int s = 1; s <= ns; s++) {
      if ( isWeaponAvailable(s) ) n++;
   }
   return n;
}

// Selected station number or zero if no station is selected
unsigned int Stores::getSelectedStation() const
{
   return selected;
}

// Default weapon availability function
bool Stores::isWeaponAvailable(const unsigned int s) const
{
   // Map 's' to a station array index
   int idx{mapSta2Idx(s)};

   // get the weapon
   bool isAvail{};
   if (idx >= 0 && weaponTbl[idx] != nullptr) {
      const IWeapon* wpn{weaponTbl[idx]->getPointer()};

      // Reasons why the weapon may not be available ...
      bool notAvail{wpn->isReleased() || wpn->isBlocked() || wpn->isJettisoned() || wpn->isFailed() || wpn->isHung()};

      // and it is if it is not not ;-)
      isAvail = !notAvail;

      wpn->unref();
   }
   return isAvail;
}

// Return a weapon by station (const version)
const IWeapon* Stores::getWeapon(const unsigned int s) const
{
   // Map 's' to a station array index
   int idx{mapSta2Idx(s)};

   // get the weapon
   const IWeapon* wpn = nullptr;
   if (idx >= 0 && weaponTbl[idx] != nullptr) {
      wpn = weaponTbl[idx]->getPointer();
   }
   return wpn;
}

// Return a weapon by station (const version)
IWeapon* Stores::getWeapon(const unsigned int s)
{
   // Map 's' to a station array index
   int idx{mapSta2Idx(s)};

   // get the weapon
   IWeapon* wpn{};
   if (idx >= 0 && weaponTbl[idx] != nullptr) {
      wpn = weaponTbl[idx]->getPointer();
   }
   return wpn;
}

// Return a external store by station (const version)
const IExternalStore* Stores::getExternalStore(const unsigned int s) const
{
   // Map 's' to a station array index
   int idx{mapSta2Idx(s)};

   const IExternalStore* p{};
   if (idx >= 0) p = esTbl[idx].getRefPtr();

   return p;
}

// Return a external store by station (const version)
IExternalStore* Stores::getExternalStore(const unsigned int s)
{
   // Map 's' to a station array index
   int idx{mapSta2Idx(s)};

   IExternalStore* p{};
   if (idx >= 0) p = esTbl[idx].getRefPtr();

   return p;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Set the number of stations on this launcher
bool Stores::setNumberOfStations(const unsigned int n)
{
   ns = n;
   return true;
}


// Select station number 's'; even if its weapon is not available for
// release.  If 's' is an invalid then the station remains unchanged and
// false is returned.
bool Stores::selectStation(const unsigned int s)
{
   bool ok{};
   if (s >= 1 && s <= ns) {
      selected = s;
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Default function to pre-release a weapon
//------------------------------------------------------------------------------

// By weapon
IWeapon* Stores::prereleaseWeapon(IWeapon* const wpn)
{
   IWeapon* flyout{};

   Player* own{getOwnship()};
   if (wpn != nullptr && own != nullptr) {

      // Release the weapon
      wpn->setLaunchVehicle(own);
      flyout = wpn->prerelease();

   }

   return flyout;
}

// By station
IWeapon* Stores::prereleaseWeapon(const unsigned int s)
{
   IWeapon* flyout{};

   IWeapon* wpn{getWeapon(s)};
   if (wpn != nullptr) {
      flyout = prereleaseWeapon(wpn);
      wpn->unref();
   }

   return flyout;
}

//------------------------------------------------------------------------------
// Default functions to release a weapon
//------------------------------------------------------------------------------

// By weapon
IWeapon* Stores::releaseWeapon(IWeapon* const wpn)
{
   IWeapon* flyout{};

   Player* own{getOwnship()};
   if (wpn != nullptr && own != nullptr) {

      // Release the weapon
      wpn->setLaunchVehicle(own);
      flyout = wpn->release();

   }

   return flyout;
}

// By station
IWeapon* Stores::releaseWeapon(const unsigned int s)
{
   IWeapon* flyout{};

   IWeapon* wpn{getWeapon(s)};
   if (wpn != nullptr) {
      flyout = releaseWeapon(wpn);
      wpn->unref();
   }

   return flyout;
}

//------------------------------------------------------------------------------
// Default function to update the weapon blocked flags
//------------------------------------------------------------------------------
void Stores::updateBlockedFlags()
{
}

//------------------------------------------------------------------------------
// Default function to jettison all jettisonable stores
//------------------------------------------------------------------------------
bool Stores::jettisonAll()
{
   // Notify the external stores that we're shutting down
   base::PairStream* list{getStores()};
   if (list != nullptr) {
      base::List::Item* item{list->getFirstItem()};
      while (item != nullptr) {
         base::Pair* pair{static_cast<base::Pair*>(item->getValue())};
         base::IComponent* p{static_cast<base::IComponent*>( pair->object() )};
         p->event(JETTISON_EVENT);
         item = item->getNext();
      }
      list->unref();
      list = nullptr;
   }
   return true;
}

//------------------------------------------------------------------------------
// assignWeaponToStation() --
// Station numbers range from 1 to getNumberOfStations()
//------------------------------------------------------------------------------
bool Stores::assignWeaponToStation(const unsigned int s, IWeapon* const wpnPtr)
{
   bool ok{};
   if (s >= 1 && s <= ns) {

      const unsigned int idx{s-1};

      // Clear previous weapon (if any)
      if (weaponTbl[idx] != nullptr) {
         numWpn--;
         weaponTbl[idx]->setLauncher(nullptr, 0);
         weaponTbl[idx] = nullptr;
      }

      // Assign the new weapon (if any)
      if (wpnPtr != nullptr) {
         weaponTbl[idx] = wpnPtr;
         weaponTbl[idx]->setLauncher(this,s);
         numWpn++;
      }

      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// assignExtStoreToStation() --
//------------------------------------------------------------------------------
bool Stores::assignExtStoreToStation(const unsigned int s, IExternalStore* const esPtr)
{
   bool ok{};
   if (s >= 1 && s <= ns) {

      const unsigned int idx{s-1};

      // Clear previous weapon (if any)
      if (esTbl[idx] != nullptr) {
         numEs--;
         esTbl[idx] = nullptr;
      }

      // Assign the new weapon (if any)
      if (esPtr != nullptr) {
         esTbl[idx] = esPtr;
         numEs++;
      }

      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// resetStores() -- Reset all stores
//------------------------------------------------------------------------------
void Stores::resetStores(base::PairStream* const list)
{
   // Reset the external stores
   if (list != nullptr) {
      base::List::Item* item{list->getFirstItem()};
      while (item != nullptr) {
         base::Pair* pair{static_cast<base::Pair*>(item->getValue())};
         base::IComponent* p{static_cast<base::IComponent*>( pair->object() )};
         p->event(RESET_EVENT);
         item = item->getNext();
      }
   }
}

//------------------------------------------------------------------------------
// Event handlers
//------------------------------------------------------------------------------

// Default weapon jettison event handler
bool Stores::onJettisonEvent(IWeapon* const wpn)
{
   bool ok{};
   if (wpn != nullptr) {

      base::PairStream* list{getStores()};
      if (list != nullptr) {

         // First, make sure it's one of ours!
         bool found{};
         base::List::Item* item{list->getFirstItem()};
         while (item != nullptr && !found) {
            base::Pair* pair{static_cast<base::Pair*>(item->getValue())};
            found = (wpn == pair->object());  // is it a match?
            item = item->getNext();
         }

         if (found) {
            // Send a jettison event to the weapon
            ok = wpn->event(JETTISON_EVENT);
         }

         list->unref();
         list = nullptr;
      }
   }
   return ok;
}

// Default external equipment jettison event handler
bool Stores::onJettisonEvent(IExternalStore* const sys)
{
   bool ok{};
   if (sys != nullptr) {

      base::PairStream* list{getStores()};
      if (list != nullptr) {

         // First, make sure it's one of ours!
         bool found{};
         base::List::Item* item{list->getFirstItem()};
         while (item != nullptr && !found) {
            base::Pair* pair{static_cast<base::Pair*>(item->getValue())};
            found = (sys == pair->object());  // is it a match?
            item = item->getNext();
         }

         if (found) {
            // Send a jettison event to the system
            ok = sys->event(JETTISON_EVENT);
         }

         list->unref();
         list = nullptr;
      }
   }
   return ok;
}

// Number of station
bool Stores::setSlotNumStations(base::Integer* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int v{msg->asInt()};
      if (v >= 1 && v <= MAX_STATIONS) {
         ok = setNumberOfStations( static_cast<unsigned int>(v) );
      } else {
         std::cerr << "Stores::setSlotnumStations() invalid number of stations: " << v;
         std::cerr << "; use [ 1 .. " << MAX_STATIONS << " ]" << std::endl;
      }
   }
   return ok;
}

// Set the stores
bool Stores::setSlotStores(const base::PairStream* const msg)
{
   // ---
   // Quick out if the number of stations hasn't been set.
   // ---
   if (ns == 0 && msg != nullptr) {
      std::cerr << "Stores::setSlotStation() Number of stations is not set!" << std::endl;
      return false;
   }

   // ---
   // Clear the previous stores and assigned weapons
   // ---
   storesList = nullptr;
   for (unsigned int s = 1; s <= ns; s++) {
      assignWeaponToStation(s, nullptr);
      assignExtStoreToStation(s, nullptr);
   }
   numWpn = 0;
   numEs = 0;

   // ---
   // Quick out if 'msg' is zero
   // ---
   if (msg == nullptr) return true;

   bool ok{true};

   // ---
   // Create the new external stores list
   //
   // For all items in the 'msg' list ...
   //   -- Make sure that it's a weapon or other type of external store, and
   //      that it has a valid station number.
   //   -- Clone the store and if it's a weapon then assign it to the station.
   // ---
   const auto newStores = new base::PairStream();

   const base::List::Item* item{msg->getFirstItem()};
   while (item != nullptr) {

      const base::Pair* pair{static_cast<const base::Pair*>(item->getValue())};
      const base::IComponent* p{static_cast<const base::IComponent*>(pair->object())};
      if (p != nullptr) {

         // get the station number from the stores' slot name
         int stationNumber{};
         const std::string& stationName{pair->slot()};
         if (base::isInteger(stationName)) {
            stationNumber = base::getInteger(stationName);
         }

         if (stationNumber > 0 && stationNumber <= static_cast<int>(ns)) {

            // check the type of component
            bool isWpn{p->isClassType(typeid(IWeapon))};
            bool isEE{p->isClassType(typeid(IExternalStore))};

            if ( isWpn || isEE ) {
               // Clone the weapon pair and set us as its container
               base::Pair* cpair{pair->clone()};
               IComponent* cp{static_cast<IComponent*>(cpair->object())};
               cp->container(this);

               if ( isWpn ) {
                  // Weapon types ...

                  // Assign the weapon to the station
                  const auto cwpn = static_cast<IWeapon*>( cpair->object() );
                  assignWeaponToStation(stationNumber, cwpn);

               }

               if ( isEE ) {
                  // External stores types ...

                  // Assign the external store to the station
                  IExternalStore* cwpn{static_cast<IExternalStore*>( cpair->object() )};
                  assignExtStoreToStation(stationNumber, cwpn);
               }

               // Add to the new stores list
               newStores->put(cpair);
               cpair->unref(); // the new list has it.
            }
            else {
               std::cerr << "Stores::setSlotStores(): invalid external stores type; use Weapon or Stores classes" << std::endl;
               ok = false;
            }

         }
         else {
            std::cerr << "Stores::setSlotStores(): invalid station number from the store's slot name." << std::endl;
            ok = false;
         }
      }

      item = item->getNext();
   }

   // Make the new stores list the active list
   if (ok && newStores->entries() > 0) {
      storesList = newStores;
   } else {
      for (unsigned int s = 1; s <= ns; s++) {
         assignWeaponToStation(s, nullptr);
      }
      numWpn = 0;
   }

   newStores->unref();

   return ok;
}

// Set the selected station number
bool Stores::setSlotSelected(base::Integer* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int v{msg->asInt()};
      if (v >= 1 && v <= static_cast<int>(ns)) {
         ok = selectStation( static_cast<unsigned int>(v) );
      } else if (ns == 0) {
         std::cerr << "Stores::setSlotStation() Number of stations is not set!" << std::endl;
      } else {
         std::cerr << "Stores::setSlotStation() invalid station number: " << v;
         std::cerr << "; use [ 1 .. " << ns << " ]" << std::endl;
      }
   }
   return ok;
}

}
}
