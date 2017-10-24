
#include "mixr/models/system/SimpleStoresMgr.hpp"

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

IMPLEMENT_SUBCLASS(SimpleStoresMgr,"StoresMgr")
EMPTY_SLOTTABLE(SimpleStoresMgr)

SimpleStoresMgr::SimpleStoresMgr()
{
   STANDARD_CONSTRUCTOR()
}

void SimpleStoresMgr::copyData(const SimpleStoresMgr& org, const bool)
{
   BaseClass::copyData(org);

   wpnRelTimer = org.wpnRelTimer;
   curWpnID = org.curWpnID;
   nCurWpn = org.nCurWpn;
}

void SimpleStoresMgr::deleteData()
{
   setSlotStores(nullptr);
}

//------------------------------------------------------------------------------
// process() -- Process phase
//------------------------------------------------------------------------------
void SimpleStoresMgr::process(const double dt)
{
   BaseClass::process(dt);

   // Weapon released timer
   if (wpnRelTimer > 0.0) {
      // decrease timer to zero
      wpnRelTimer -= dt;
   }
}

//------------------------------------------------------------------------------
// updateData() -- update non-time critical stuff here
//------------------------------------------------------------------------------
void SimpleStoresMgr::updateData(const double dt)
{
   BaseClass::updateData(dt);

   // ---
   // Get the current weapon data
   // ---
   {
      auto wpn = static_cast<AbstractWeapon*>(getCurrentWeapon());
      if (wpn != nullptr) {
         // Weapon ID
         curWpnID = wpn->getWeaponID();

         // Number of this type weapon
         int count{};
         base::PairStream* list{getStores()};
         if (list != nullptr) {
            const base::List::Item* item{list->getFirstItem()};
            while (item != nullptr) {
               const base::Pair* pair{static_cast<const base::Pair*>(item->getValue())};
               if (pair != nullptr) {
                  const auto s = dynamic_cast<const AbstractWeapon*>( pair->object() );
                  if ( s != nullptr && s->isMode(Player::INACTIVE) && std::strcmp(s->getFactoryName(), wpn->getFactoryName()) == 0 ) {
                     count++;
                  }
               }
               item = item->getNext();
            }
            list->unref();
            list = nullptr;
         }
         nCurWpn = count;

         wpn->unref();
         wpn = nullptr;
      }
      else {
         curWpnID = 0;
         nCurWpn = 0;
      }
   }

}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

// Default function to return the current weapon (Pre-ref()'d)
AbstractWeapon* SimpleStoresMgr::getCurrentWeapon()
{
   AbstractWeapon* wpn{};
   if ( isWeaponDeliveryMode(A2A) ) {
      wpn = getNextMissile();    // We need a missile
   } else {
      wpn = getNextBomb();       // We need a bomb
   }
   return wpn;
}

const AbstractWeapon* SimpleStoresMgr::getCurrentWeapon() const
{
   const AbstractWeapon* wpn{};
   if ( isWeaponDeliveryMode(A2A) ) {
      wpn = getNextMissile();    // We need a missile
   } else {
      wpn = getNextBomb();       // We need a bomb
   }
   return wpn;
}

// Get the current weapon ID
int SimpleStoresMgr::getCurrentWeaponID() const
{
   return curWpnID;
}

// Get the number of weapons of the current type
int SimpleStoresMgr::getNumCurrentWeapons() const
{
   return nCurWpn;
}

// Are weapons are being released
bool SimpleStoresMgr::isWeaponReleased() const
{
   return (wpnRelTimer > 0.0);
}

// ---
// Default function to get the next free missile in our stores (Pre-ref()'d)
// ---
Missile* SimpleStoresMgr::getNextMissile()
{
   return getNextMissileImp();
}

const Missile* SimpleStoresMgr::getNextMissile() const
{
   return (const_cast<SimpleStoresMgr*>(this))->getNextMissileImp();
}

Missile* SimpleStoresMgr::getNextMissileImp()
{
   Missile* msl{};

   base::PairStream* list{getWeapons()};
   if (list != nullptr) {

      // find the first free (inactive) missile
      base::List::Item* item{list->getFirstItem()};
      while (item != nullptr && msl == nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto p = dynamic_cast<Missile*>(pair->object());
         if (p != nullptr) {
            if (p->isInactive() || p->isReleaseHold()) {
               msl = static_cast<Missile*>(p->getPointer());
            }
         }
         item = item->getNext();
      }
      list->unref();
   }

   return msl;
}

// ---
// Default function to get the next free SAM in our stores (Pre-ref()'d)
// ---
Sam* SimpleStoresMgr::getNextSam()
{
   return getNextSamImp();
}

const Sam* SimpleStoresMgr::getNextSam() const
{
   return (const_cast<SimpleStoresMgr*>(this))->getNextSamImp();
}

Sam* SimpleStoresMgr::getNextSamImp()
{
   Sam* msl{};

   base::PairStream* list{getWeapons()};
   if (list != nullptr) {

      // find the first free (inactive) SAM
      base::List::Item* item{list->getFirstItem()};
      while (item != nullptr && msl == nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto p = dynamic_cast<Sam*>(pair->object());
         if (p != nullptr) {
            if (p->isInactive() || p->isReleaseHold()) {
               msl = static_cast<Sam*>(p->getPointer());
            }
         }
         item = item->getNext();
      }

      list->unref();
   }

   return msl;
}

// ---
// Default function to get the next free bomb in our stores (Pre-ref()'d)
// ---
Bomb* SimpleStoresMgr::getNextBomb()
{
   return getNextBombImp();
}

const Bomb* SimpleStoresMgr::getNextBomb() const
{
   return (const_cast<SimpleStoresMgr*>(this))->getNextBombImp();
}

Bomb* SimpleStoresMgr::getNextBombImp()
{
   Bomb* bomb{};

   base::PairStream* list{getWeapons()};
   if (list != nullptr) {

      // find the first free (inactive) bomb
      base::List::Item* item{list->getFirstItem()};
      while (item != nullptr && bomb == nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto p = dynamic_cast<Bomb*>(pair->object());
         if (p != nullptr) {
            if (p->isInactive() || p->isReleaseHold()) {
               bomb = static_cast<Bomb*>(p->getPointer());
            }
         }
         item = item->getNext();
      }

      list->unref();
   }

   return bomb;
}

// ---
// Default function to get the next free chaff bundle in our stores (Pre-ref()'d)
// ---
Chaff* SimpleStoresMgr::getNextChaff()
{
   return getNextChaffImp();
}

const Chaff* SimpleStoresMgr::getNextChaff() const
{
   return (const_cast<SimpleStoresMgr*>(this))->getNextChaffImp();
}

Chaff* SimpleStoresMgr::getNextChaffImp()
{
   Chaff* chaff{};

   base::PairStream* list{getWeapons()};
   if (list != nullptr) {

      // find the first free (inactive) chaff bundle
      base::List::Item* item{list->getFirstItem()};
      while (item != nullptr && chaff == nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto p = dynamic_cast<Chaff*>(pair->object());
         if (p != nullptr) {
            if (p->isInactive() || p->isReleaseHold()) {
               chaff = static_cast<Chaff*>(p->getPointer());
            }
         }
         item = item->getNext();
      }

      list->unref();
   }

   return chaff;
}

// ---
// Default function to get the next free flare in our stores (Pre-ref()'d)
// ---
Flare* SimpleStoresMgr::getNextFlare()
{
   return getNextFlareImp();
}

const Flare* SimpleStoresMgr::getNextFlare() const
{
   return (const_cast<SimpleStoresMgr*>(this))->getNextFlareImp();
}

Flare* SimpleStoresMgr::getNextFlareImp()
{
   Flare* flare{};

   base::PairStream* list{getWeapons()};
   if (list != nullptr) {

      // find the first free (inactive) flare
      base::List::Item* item{list->getFirstItem()};
      while (item != nullptr && flare == nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto p = dynamic_cast<Flare*>(pair->object());
         if (p != nullptr) {
            if (p->isInactive() || p->isReleaseHold()) {
               flare = static_cast<Flare*>(p->getPointer());
            }
         }
         item = item->getNext();
      }

      list->unref();
   }

   return flare;
}

// ---
// Default function to get the next decoy from our stores (Pre-ref()'d)
// ---
Decoy* SimpleStoresMgr::getNextDecoy()
{
   return getNextDecoyImp();
}

const Decoy* SimpleStoresMgr::getNextDecoy() const
{
   return (const_cast<SimpleStoresMgr*>(this))->getNextDecoyImp();
}

Decoy* SimpleStoresMgr::getNextDecoyImp()
{
   Decoy* decoy{};

   base::PairStream* list{getWeapons()};
   if (list != nullptr) {

      // find the first free (inactive) decoy
      base::List::Item* item{list->getFirstItem()};
      while (item != nullptr && decoy == nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto p = dynamic_cast<Decoy*>( pair->object() );
         if (p != nullptr) {
            if (p->isInactive() || p->isReleaseHold()) {
               decoy = static_cast<Decoy*>(p->getPointer());
            }
         }
         item = item->getNext();
      }

      list->unref();
   }

   return decoy;
}

// Get the next free missile of type 'missileType'
Missile* SimpleStoresMgr::getSpecificMissile(const base::String* const missileType)
{
   Missile* msl{};
   if (missileType != nullptr) {

      base::PairStream* list{getWeapons()};
      if (list != nullptr) {

         // Find the first free (inactive) missile of type weaponType
         base::List::Item* item{list->getFirstItem()};
         while (item != nullptr && msl == nullptr) {
            const auto pair = static_cast<base::Pair*>(item->getValue());
            const auto p = dynamic_cast<Missile*>(pair->object());
            if (p != nullptr && p->isInactive()) {
               // Ok, we have a missile, but is it the type we want?
               if (*p->getType() == *missileType) {
                  p->ref();
                  msl = p;
               }
            }
            item = item->getNext();
         }

         list->unref();
      }

   }
   return msl;
}

// Get the next free bomb of type 'bombType'
Bomb* SimpleStoresMgr::getSpecificBomb(const base::String* const bombType)
{
   Bomb* bomb{};
   if (bombType != nullptr)  {

      base::PairStream* list{getWeapons()};
      if (list != nullptr)  {

         // Find the first free (inactive) bomb
         base::List::Item* item{list->getFirstItem()};
         while (item != nullptr && bomb == nullptr) {
            const auto pair = static_cast<base::Pair*>(item->getValue());
            const auto p = dynamic_cast<Bomb*>(pair->object());
            if (p != nullptr && p->isInactive()) {
               // Ok, we have a bomb, but is it the type we want?
               if (*p->getType() == *bombType) {
                  p->ref();
                  bomb = p;
               }
            }
            item = item->getNext();
         }

         list->unref();
      }

   }
   return bomb;
}

// Get the next free weapon of this 'type'
AbstractWeapon* SimpleStoresMgr::getSpecificWeapon(const std::type_info& type)
{
   AbstractWeapon* wpn{};
   base::PairStream* list{getWeapons()};
   if (list != nullptr) {
      // Find the first free (inactive) bomb
      base::List::Item* item{list->getFirstItem()};
      while (item != nullptr && wpn == nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto p = dynamic_cast<AbstractWeapon*>(pair->object());
         if (p != nullptr && p->isInactive() && p->isClassType(type)) {
            p->ref();
            wpn = p;
         }
         item = item->getNext();
      }
      list->unref();
   }
   return wpn;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the release timer
bool SimpleStoresMgr::setWeaponReleaseTimer(const double v)
{
   wpnRelTimer = v;
   return true;
}

//------------------------------------------------------------------------------
// Find the next target to shoot
//------------------------------------------------------------------------------
Track* SimpleStoresMgr::getNextTarget()
{
   Track* trk{};

   if (getOwnship() != nullptr) {

      OnboardComputer* obc{getOwnship()->getOnboardComputer()};
      if (obc != nullptr) {

         // Get the next to shoot
         int n{};
         base::safe_ptr<Track> track;
         n = obc->getShootList(&track,1);
         if (n > 0) trk = track;

      }
   }
   return trk;
}

//------------------------------------------------------------------------------
// Release one Missile
//------------------------------------------------------------------------------
Missile* SimpleStoresMgr::releaseOneMissile()
{
   Missile* flyout{};
   Missile* wpn{getNextMissile()};
   if (wpn != nullptr) {
      flyout = static_cast<Missile*>( releaseWeapon(wpn) );
   }
   return flyout;
}

//------------------------------------------------------------------------------
// Release one SAM
//------------------------------------------------------------------------------
Sam* SimpleStoresMgr::releaseOneSam()
{
   Sam* flyout{};
   Sam* wpn{getNextSam()};
   if (wpn != nullptr) {
      flyout = static_cast<Sam*>( releaseWeapon(wpn) );
   }
   return flyout;
}

//------------------------------------------------------------------------------
// Release one Bomb
//------------------------------------------------------------------------------
Bomb* SimpleStoresMgr::releaseOneBomb()
{
   Bomb* flyout{};
   Bomb* wpn{getNextBomb()};
   if (wpn != nullptr) {
      flyout = static_cast<Bomb*>( releaseWeapon(wpn) );
   }
   return flyout;
}

//------------------------------------------------------------------------------
// Release one chaff bundle
//------------------------------------------------------------------------------
Chaff* SimpleStoresMgr::releaseOneChaff()
{
   Chaff* flyout{};
   Chaff* chaff{getNextChaff()};
   if (chaff != nullptr) {
      flyout = static_cast<Chaff*>( releaseWeapon(chaff) );
   }
   return flyout;
}

//------------------------------------------------------------------------------
// Release one flare
//------------------------------------------------------------------------------
Flare* SimpleStoresMgr::releaseOneFlare()
{
   Flare* flyout{};
   Flare* flare{getNextFlare()};
   if (flare != nullptr) {
      flyout = static_cast<Flare*>( releaseWeapon(flare) );
   }
   return flyout;
}

//------------------------------------------------------------------------------
// Release one decoy
//------------------------------------------------------------------------------
Decoy* SimpleStoresMgr::releaseOneDecoy()
{
   Decoy* flyout{};
   Decoy* decoy{getNextDecoy()};
   if (decoy != nullptr) {
      flyout = static_cast<Decoy*>( releaseWeapon(decoy) );
   }
   return flyout;
}

//------------------------------------------------------------------------------
// Manage the weapon release event
//------------------------------------------------------------------------------
bool SimpleStoresMgr::onWpnRelEvent(const base::Boolean* const sw)
{
   // Weapon release follows the switch or by default is true
   bool wpnRel{true};
   if (sw != nullptr) wpnRel = sw->getBoolean();

   if ( wpnRel && (isWeaponDeliveryMode(A2A) || isWeaponDeliveryMode(A2G)) ) {

      // A/A missiles and A/G bombs only ...

      AbstractWeapon* wpn{getCurrentWeapon()};
      if (wpn != nullptr) {

         // release the weapon ---
         //  if successful, returns a pre-ref()'d pointer to the flyout weapon.
         AbstractWeapon* flyout{releaseWeapon(wpn)};
         if (flyout != nullptr) {

            if (isWeaponDeliveryMode(A2A)) {

               // ---
               // Setup the guidance ...
               // ---
               Track* trk{getNextTarget()};
               if (trk != nullptr) {
                  Player* tgt{trk->getTarget()};

                  TrackManager* tm{};
                  RfSensor* sm{flyout->getSensor()};
                  if (sm != nullptr) tm = sm->getTrackManager();

                  if (tm != nullptr) {
                     // Give a copy of the track to the weapon's track manager
                     Track* newTrk{trk->clone()};
                     tm->clearTracksAndQueues();
                     tm->addTrack(newTrk);
                     flyout->setTargetTrack(newTrk,true);
                  }
                  else if (tgt != nullptr) {
                     // No sensor -- fake it and just track the target
                     flyout->setTargetPlayer(tgt,true);
                  }
                  else {
                     // no target track
                  }

                  trk->setWeaponReleased(true);
               }

            }

            // Set the weapon release timer
            setWeaponReleaseTimer(1.0);
         }

         if (flyout != nullptr) flyout->unref(); // unref() the flyout

         wpn->unref();
         wpn = nullptr;
      }

   }
   else {
      setWeaponReleaseTimer(0);
   }

   return true;
}


//------------------------------------------------------------------------------
// Manage the trigger switch event
//------------------------------------------------------------------------------
bool SimpleStoresMgr::onTriggerSwEvent(const base::Boolean* const sw)
{
   Gun* g{getGun()};    // Get the primary gun
   if (g != nullptr) {

      // Single Burst?
      bool burst{sw == nullptr};

      // Firing?
      bool fire{};
      if ( isWeaponDeliveryMode(A2A) || isWeaponDeliveryMode(A2G) ) {
         if ( burst ) fire = true;
         else fire = sw->getBoolean();
      }

      // Pass the control to the gun
      g->fireControl(fire, burst);
   }

   return true;
}

}
}
