
#include "mixr/models/player/weapon/IWeapon.hpp"

#include "mixr/models/dynamics/IDynamics.hpp"
#include "mixr/models/player/IPlayer.hpp"
#include "mixr/models/Designator.hpp"
#include "mixr/models/system/Gun.hpp"
#include "mixr/models/system/IStores.hpp"
#include "mixr/models/track/ITrack.hpp"
#include "mixr/models/WorldModel.hpp"

#include "mixr/simulation/IDataRecorder.hpp"

#include "mixr/base/IList.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/String.hpp"
#include "mixr/base/osg/Matrixd"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/INumber.hpp"

#include "mixr/base/units/angles.hpp"
#include "mixr/base/units/lengths.hpp"
#include "mixr/base/units/times.hpp"

#include "mixr/base/util/nav_utils.hpp"

namespace mixr {
namespace models {

IMPLEMENT_ABSTRACT_SUBCLASS(IWeapon, "IWeapon")

// parameters
const double IWeapon::DEFAULT_MAX_TGT_RNG     {2000.0};    // meters
const double IWeapon::DEFAULT_MAX_TGT_LOS_ERR {1.0};       // radians

BEGIN_SLOTTABLE(IWeapon)
    "released",         //  1: Weapon has been released
    "failed",           //  2: Weapon failed (e.g., reasonableness Test)
    "power",            //  3: Weapon power flag
    "hang",             //  4: Will be a hung store
    "hung",             //  5: Hung store
    "maxTOF",           //  6: max time of flight        (sec)
    "tsg",              //  7: time to start guidance    (sec - tof)
    "maxBurstRng",      //  8: max burst rng             (meters)
    "lethalRange",      //  9: lethal range              (meters)
    "sobt",             // 10: start-of-burn time        (sec - tof)
    "eobt",             // 11: end-of-burn time          (sec - tof)
    "maxGimbal",        // 12: max gimbal angle          (base::Angle)
    "tgtPos",           // 13: TEST target position [ n e d ] (meters)
    "weaponID",         // 14: Weapon type ID (user defined number)
    "dummy",            // 15: Dummy store (launch, but don't flyout or detonate)
    "jettisonable",     // 16: Weapon can be jettisoned (default: true)
    "testTgtName"       // 17: TEST only: target player name
END_SLOTTABLE(IWeapon)

BEGIN_SLOT_MAP(IWeapon)
    ON_SLOT( 1,  setSlotReleased,    base::Boolean)
    ON_SLOT( 2,  setSlotFailed,      base::Boolean)
    ON_SLOT( 3,  setSlotPower,       base::Boolean)
    ON_SLOT( 4,  setSlotWillHang,    base::Boolean)
    ON_SLOT( 5,  setSlotHung,        base::Boolean)

    ON_SLOT( 6,  setSlotMaxTOF,      base::ITime)
    ON_SLOT( 6,  setSlotMaxTOF,      base::INumber)

    ON_SLOT( 7,  setSlotTSG,         base::ITime)
    ON_SLOT( 7,  setSlotTSG,         base::INumber)

    ON_SLOT( 8,  setSlotMaxBurstRng, base::ILength)
    ON_SLOT( 8,  setSlotMaxBurstRng, base::INumber)

    ON_SLOT( 9, setSlotLethalRange,  base::ILength)
    ON_SLOT( 9, setSlotLethalRange,  base::INumber)

    ON_SLOT(10, setSlotSOBT,         base::ITime)
    ON_SLOT(10, setSlotSOBT,         base::INumber)

    ON_SLOT(11, setSlotEOBT,         base::ITime)
    ON_SLOT(11, setSlotEOBT,         base::INumber)

    ON_SLOT(12, setSlotMaxGimbal,    base::IAngle)

    ON_SLOT(13, setSlotTgtPos,       base::IList)
    ON_SLOT(14, setSlotWeaponID,     base::Integer)
    ON_SLOT(15, setSlotDummy,        base::Boolean)
    ON_SLOT(16, setSlotJettisonable, base::Boolean)
    ON_SLOT(17, setSlotTestTgtName,  base::String)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(IWeapon)
    ON_EVENT_OBJ(DESIGNATOR_EVENT, onDesignatorEvent, Designator)
    ON_EVENT( JETTISON_EVENT, onJettisonEvent)
END_EVENT_HANDLER()

IWeapon::IWeapon()
{
   STANDARD_CONSTRUCTOR()

   static base::String generic("GenericWeapon");
   setType_old(&generic);
   setType("GenericWeapon");

   setMode(Mode::INACTIVE);
   setInitMode(Mode::INACTIVE);

   initData();
}

void IWeapon::initData()
{
   tgtPos.set(0,0,0);
   tgtVel.set(0,0,0);
   tgtDetLoc.set(0,0,0);
   setMaxGimbalAngle(30.0 * static_cast<double>(base::angle::D2RCC));
}

void IWeapon::copyData(const IWeapon& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   setFlyoutWeapon(nullptr);
   setInitialWeapon(nullptr);
   setTargetTrack(nullptr, false);
   setTargetPlayer(nullptr, false);
   setLauncher(nullptr, 0);
   setLaunchVehicle(nullptr);

   tgtPos          = org.tgtPos;
   tgtVel          = org.tgtVel;
   tgtPosValid     = org.tgtPosValid;
   posTrkEnb       = org.posTrkEnb;
   maxTgtRng       = org.maxTgtRng;
   maxTgtLosErr    = org.maxTgtLosErr;
   detonationRange = org.detonationRange;
   tgtDetLoc       = org.tgtDetLoc;
   station         = org.station;
   weaponID        = org.weaponID;
   eventID         = org.eventID;
   power           = org.power;
   failed          = org.failed;
   released        = org.released;
   releaseHold     = org.releaseHold;
   willHang        = org.willHang;
   hung            = org.hung;
   blocked         = org.blocked;
   canJettison     = org.canJettison;
   jettisoned      = org.jettisoned;
   dummyFlg        = org.dummyFlg;
   results         = org.results;
   tstTgtNam       = org.tstTgtNam;

   tof             = org.tof;
   maxTOF          = org.maxTOF;
   tsg             = org.tsg;
   maxBurstRng     = org.maxBurstRng;
   lethalRange     = org.lethalRange;
   eobt            = org.eobt;
   sobt            = org.sobt;
   maxGimbal       = org.maxGimbal;
}

void IWeapon::deleteData()
{
   setFlyoutWeapon(nullptr);
   setInitialWeapon(nullptr);
   setTargetTrack(nullptr, false);
   setTargetPlayer(nullptr, false);
   setLauncher(nullptr, 0);
   setLaunchVehicle(nullptr);
}

//------------------------------------------------------------------------------
// reset() -- Reset vehicle
//------------------------------------------------------------------------------
void IWeapon::reset()
{
   BaseClass::reset();

   IWeapon* flyout{getFlyoutWeapon()};

   // If there's a flyout weapon still in PRE_RELEASE then reset it
   if (flyout != nullptr && flyout != this && flyout->isMode(Mode::PRE_RELEASE) ) {
      flyout->reset();
   }

   // If this is flyout weapon then set the mode to DELETE_REQUEST to
   // remove us from the player list.
   if (flyout == this) {
      setMode(Mode::DELETE_REQUEST);
   }

   hung       = false;
   released   = false;
   failed     = false;
   jettisoned = false;

   setDetonationResults(Detonation::NONE);

   setFlyoutWeapon(nullptr);
   setInitialWeapon(nullptr);
   setTargetTrack(nullptr, false);
   setTargetPlayer(nullptr, false);

   // launch vehicle
   if ( ( getLaunchVehicle() == nullptr ) && ( flyout != this ) ) {
      setLaunchVehicle( static_cast<IPlayer*>(findContainerByType( typeid(IPlayer) )) );
   }

   // Test player?
   if (tstTgtNam != nullptr) {
      WorldModel* s = getWorldModel();
      if (s != nullptr) {
         const auto t = dynamic_cast<IPlayer*>(s->findPlayerByName( (*tstTgtNam).c_str() ));   // added DDH
         if (t != nullptr) setTargetPlayer(t, true);
     }
   }

   setTOF(0.0);

   if (flyout != nullptr) flyout->unref();
}

//------------------------------------------------------------------------------
// updateTC() -- update time critical stuff here
//------------------------------------------------------------------------------
void IWeapon::updateTC(const double dt)
{
   BaseClass::updateTC(dt);

   unsigned int ph{getWorldModel()->phase()};

   // Phase #0 -- Transition from pre-release to active at the end of dynamics
   // phase (after the call to BaseClass), so that our position, which was
   // relative to our launch vehicle, has been computed.
   if (ph == 0 && isMode(Mode::PRE_RELEASE) && !isReleaseHold() ) {
      atReleaseInit();
      setMode(Mode::ACTIVE);
   }

   // Phase #3
   if (ph == 3 && isActive() && isLocalPlayer() && !isJettisoned() && !isDummy()) {

      // Simple function to get target coordinates
      if (posTrkEnb) positionTracking();

      // Update our Time-Of-Flight (TOF)
      if (isMode(Mode::ACTIVE)) updateTOF(dt * 4.0);
   }
}

//------------------------------------------------------------------------------
// dynamics() -- update vehicle dynamics
//------------------------------------------------------------------------------
void IWeapon::dynamics(const double dt)
{
   if (isMode(Mode::PRE_RELEASE)) {
      // Weapon is on the same side as the launcher
      setSide( getLaunchVehicle()->getSide() );

      // Launch vehicles rotational matrix
      base::Matrixd lvM{getLaunchVehicle()->getRotMat()};

      // Set weapon's position at launch
      // 1) Weapon's position is its position relative to the launcher (launcher's body coordinates)
      // 2) Rotate to earth coordinates
      // 3) Add the launcher's position
      const base::Vec2d ip{getInitPosition()};
      const base::Vec3d pos0b(ip.x(), ip.y(), -getInitAltitude());
      const base::Vec3d pos0e{pos0b * lvM}; // body to earth
      const base::Vec3d lpos{getLaunchVehicle()->getPosition()};
      const base::Vec3d pos1{lpos + pos0e};
      setPosition( pos1 );

      // Weapon's orientation at launch
      const base::Vec3d ia{getInitAngles()};
      base::Matrixd rr;
      base::nav::computeRotationalMatrix( ia[0], ia[1], ia[2], &rr);
      rr *= lvM;

      setRotMat(rr);

      // Set velocities are the same as the launcher
      setVelocity( getLaunchVehicle()->getVelocity() );

      // Not accelerations or angular velocities
      setAcceleration( 0, 0, 0 );
      setAngularVelocities( 0, 0, 0 );
   } else if (!isJettisoned()) {

      if (isLocalPlayer() && !isDummy() && getDynamicsModel() == nullptr) {
         // Use our default (simple) weapon model
         weaponGuidance(dt);
         weaponDynamics(dt);
      }
      BaseClass::dynamics(dt);

   }
}

//------------------------------------------------------------------------------
// shutdownNotification() -- We're shutting down
//------------------------------------------------------------------------------
bool IWeapon::shutdownNotification()
{
   // Clear all of our pointers
   setFlyoutWeapon(nullptr);
   setInitialWeapon(nullptr);
   setTargetTrack(nullptr, false);
   setTargetPlayer(nullptr, false);
   setLauncher(nullptr, 0);
   setLaunchVehicle(nullptr);

   return BaseClass::shutdownNotification();
}

//-----------------------------------------------------------------------------
// getMajorType() -- Returns the player's major type
//-----------------------------------------------------------------------------
unsigned int IWeapon::getMajorType() const
{
   return WEAPON;
}

//------------------------------------------------------------------------------
// Default designator event handler
//------------------------------------------------------------------------------
bool IWeapon::onDesignatorEvent(const Designator* const)
{
   // In the future, we'll want to pass this to our LASER detector
   // But we don't have any yet, so yet our derived classes override
   // this one for now.
   return true;
}

//------------------------------------------------------------------------------
// Default jettison event handler --
//  -- We're setting the initial weapon's mode to LAUNCHED with the "we've been
//  jettisoned" flag, and we're removing the pre-released flyout weapon, if any.
//------------------------------------------------------------------------------
bool IWeapon::onJettisonEvent()
{
   bool ok{};
   if (!isReleased() && !isJettisoned() && isJettisonable()) {

      // If we haven't already been release or jettisoned,
      // and we can be jettisoned ...

      IWeapon* flyout{getFlyoutWeapon()};
      IWeapon* initWpn{getInitialWeapon()};

      // If there is a flyout weapon that's still in PRE_RELEASE mode
      // then call its jettison event handler.
      if (flyout != nullptr && flyout != this && flyout->isMode(Mode::PRE_RELEASE) ) {
         flyout->onJettisonEvent();
      }

      // If this is the flyout weapon then set our mode to DELETE_REQUEST
      if (flyout == this) {
         setMode(Mode::DELETE_REQUEST);
      }

      // If this is initial weapon then set our mode to LAUNCHED
      if (initWpn == this) {
         initWpn->setMode(IPlayer::Mode::LAUNCHED);
      }

      // And set the 'jettisoned' and 'released' flags
      setJettisoned(true);
      setReleased(true);

      // cleanup
      if (flyout != nullptr) flyout->unref();
      if (initWpn != nullptr) initWpn->unref();

      ok = true;
   }

   return ok;
}

//------------------------------------------------------------------------------
// Check local players for the effects of the detonation -- did we hit anyone?
//------------------------------------------------------------------------------
void IWeapon::checkDetonationEffect()
{
   WorldModel* s{getWorldModel()};
   if (s != nullptr) {
      // Only local players within 10X max burst range
      double maxRng{10.0 * getMaxBurstRng()};

      // Find our target (if any)
      const IPlayer* tgt{getTargetPlayer()};
      if (tgt == nullptr) {
         const ITrack* trk{getTargetTrack()};
         if (trk != nullptr) tgt = trk->getTarget();
      }

      base::PairStream* plist{s->getPlayers()};
      if (plist != nullptr) {
         base::IList::Item* item{plist->getFirstItem()};

         // Process the detonation for all local, in-range players
         bool finished{};
         while (item != nullptr && !finished) {
            base::Pair* pair{static_cast<base::Pair*>(item->getValue())};
            IPlayer* p{static_cast<IPlayer*>(pair->object())};
            finished = p->isProxyPlayer();  // local only
            if (!finished && (p != this) ) {
               base::Vec3d dpos{p->getPosition() - getPosition()};
               const double rng{dpos.length()};
               if ( (rng <= maxRng) || (p == tgt) ) p->processDetonation(rng, this);
            }
            item = item->getNext();
         }

         // cleanup
         plist->unref();
         plist = nullptr;
      }

   }
}

//------------------------------------------------------------------------------
// collisionNotification() -- We just impacted with another player
//------------------------------------------------------------------------------
bool IWeapon::collisionNotification(IPlayer* const other)
{
   bool ok{};

   if (!isCrashOverride() && isLocalPlayer()) {
      ok = killedNotification(other);

      // We've detonated!
      setMode(Mode::DETONATED);
      setDetonationResults(Detonation::ENTITY_IMPACT);

      // Compute detonation location relative to the other ship
      setTargetPlayer(other, false);
      setLocationOfDetonation();

      // Ground detonation -- anyone here to see it?
      checkDetonationEffect();

      // Log the event
      BEGIN_RECORD_DATA_SAMPLE( getWorldModel()->getDataRecorder(), REID_WEAPON_DETONATION )
         SAMPLE_3_OBJECTS( this, getLaunchVehicle(), getTargetPlayer() )
         SAMPLE_2_VALUES( static_cast<int>(Detonation::ENTITY_IMPACT), getDetonationRange() )
      END_RECORD_DATA_SAMPLE()
   }

   return ok;
}

//------------------------------------------------------------------------------
// crashNotification() -- We just impacted the ground
//------------------------------------------------------------------------------
bool IWeapon::crashNotification()
{
   // ---
   // We've detonated because we've hit the ground
   // ---
   bool ok{};
   if (!isCrashOverride() && isLocalPlayer()) {

      ok = killedNotification();
      setDetonationResults(Detonation::GROUND_IMPACT);
      setMode(Mode::DETONATED);

      // ---
      // Compute location of detonation relative to target
      // ---
      if (getTargetPlayer() != nullptr) {
         setLocationOfDetonation();
      }

      // ---
      // Ground detonation -- anyone here to see it?
      // ---
      checkDetonationEffect();

      // ---
      // Log the event
      // ---
      BEGIN_RECORD_DATA_SAMPLE( getWorldModel()->getDataRecorder(), REID_WEAPON_DETONATION )
         SAMPLE_3_OBJECTS( this, getLaunchVehicle(), getTargetPlayer() )
         SAMPLE_2_VALUES( static_cast<int>(Detonation::GROUND_IMPACT), getDetonationRange() )
      END_RECORD_DATA_SAMPLE()
   }

   return ok;
}

//------------------------------------------------------------------------------
// prerelease() -- prerelease this weapon.
//
// Returns a point to the flyout weapon player, which is still in 'release hold'.
//------------------------------------------------------------------------------
IWeapon* IWeapon::prerelease()
{
   IWeapon* flyout{getFlyoutWeapon()};

   // If we're not already (pre)released or jettisoned,
   //   and we'll need a launching player and a simulation
   WorldModel* sim{static_cast<WorldModel*>( findContainerByType(typeid(WorldModel)) )};
   IPlayer* lplayer{getLaunchVehicle()};
   if (!isReleased() && !isJettisoned() && flyout == nullptr && lplayer != nullptr && sim != nullptr) {

      // we'll get an event at the time of release.
      // but get an event ID to use as our player ID
      eventID = 0;

      // Next we'll clone ourself --
      //  -- this will be the actual weapon player what will do the fly-out.
      flyout = this->clone();

      flyout->container( sim );
      flyout->reset();

      flyout->setFlyoutWeapon(flyout);
      flyout->setInitialWeapon(this);
      flyout->setID( sim->getNewReleasedWeaponID() );

      flyout->setLaunchVehicle( lplayer );
      flyout->setSide( lplayer->getSide() );

      // and set the weapon prerelease
      flyout->setMode(Mode::PRE_RELEASE);
      flyout->setReleased(false);
      flyout->setReleaseHold(true);

      // Set our mode flags to fully released.
      setFlyoutWeapon(flyout);
      setInitialWeapon(this);
      setReleased(false);
      setReleaseHold(true);

      // add it to the flyout weapon player list
      char pname[32];
      std::sprintf(pname,"W%05d", flyout->getID());
      sim->addNewPlayer(pname,flyout);

   }

   return flyout;
}

//------------------------------------------------------------------------------
// release() -- release the weapon
//
//  1) We'll clone the initial weapon, which will be the flyout weapon,
//     reset the flyout weapon, set a few flags, and add the flyout weapon
//     to the player list.
//
//  2) If we already have a flyout weapon in 'release hold', then this
//      function, which can be called for either the initial or flyout
//      weapons, will set a few flags and clear the 'release hold'.
//
// Return a pointer to the flyout weapon player
//------------------------------------------------------------------------------
IWeapon* IWeapon::release()
{
   IWeapon* flyout{};

   // When this weapon isn't already released, blocked or jettisoned.
   if ( !isReleased() && !isBlocked() && !isJettisoned() ) {

      // and isn't flagged to be a hung store (i.e., failure mode),
      if (!getWillHang()) {

         // and we have a launching player and a simulation ...
         IPlayer* lplayer{getLaunchVehicle()};
         const auto sim = static_cast<WorldModel*>( findContainerByType(typeid(WorldModel)) );
         if ( lplayer != nullptr && sim != nullptr) {

            // then release the weapon!

            flyout = getFlyoutWeapon();
            if (flyout != nullptr) {
               // When we've already created a flyout weapon, which is on the
               // player list in 'release hold' ...

               // we'll just need to clear the "release
               // hold" flag, which will let the flyout weapon go ACTIVE.
               flyout->setReleased(true);
               flyout->setReleaseHold(false);

               // Set the initial weapon's mode flags to fully released.
               IWeapon* initWpn{getInitialWeapon()};
               initWpn->setMode(IPlayer::Mode::LAUNCHED);
               initWpn->setReleased(true);
               initWpn->setReleaseHold(false);
               initWpn->unref();
            } else {
               // When we haven't already created a flyout then this is
               // a direct release ...

               // Get a release event
               eventID = sim->getNewWeaponEventID();

               // Next we'll clone ourself --
               //  -- this will be the actual weapon player what will do the fly-out.
               flyout = this->clone();

               flyout->container( sim );
               flyout->reset();

               flyout->setFlyoutWeapon(flyout);
               flyout->setInitialWeapon(this);
               flyout->setID( sim->getNewReleasedWeaponID() );

               flyout->setLaunchVehicle( lplayer );
               flyout->setSide( lplayer->getSide() );

               // and set the weapon prerelease
               flyout->setMode(Mode::PRE_RELEASE);
               flyout->setReleased(true);
               flyout->setReleaseHold(false);

               // Set our mode flags to fully released.
               setFlyoutWeapon(flyout);
               setInitialWeapon(this);
               setMode(IPlayer::Mode::LAUNCHED);
               setReleased(true);
               setReleaseHold(false);

               // add it to the flyout weapon player list
               char pname[32];
               std::sprintf(pname,"W%05d", flyout->getID());
               sim->addNewPlayer(pname,flyout);
            }

            BEGIN_RECORD_DATA_SAMPLE( getWorldModel()->getDataRecorder(), REID_WEAPON_RELEASED )
               SAMPLE_3_OBJECTS( flyout, getLaunchVehicle(), nullptr )  // weapon, shooter, target
               SAMPLE_2_VALUES( 0, 0.0 )
            END_RECORD_DATA_SAMPLE()

         }

      } else {
         // We have a hung store
         setHung(true);

         BEGIN_RECORD_DATA_SAMPLE( getWorldModel()->getDataRecorder(), REID_WEAPON_HUNG )
            SAMPLE_3_OBJECTS( this, getLaunchVehicle(), nullptr )
         END_RECORD_DATA_SAMPLE()

      }
   }

   return flyout;
}

//------------------------------------------------------------------------------
// atReleaseInit() -- Init weapon data at release
//------------------------------------------------------------------------------
void IWeapon::atReleaseInit()
{
   // Set the release event
   if (eventID == 0) {
      eventID = getWorldModel()->getNewWeaponEventID();
   }

   // Reset the dynamics mode (if any)
   if (getDynamicsModel() != nullptr) {
        getDynamicsModel()->atReleaseInit();
   }
}

//------------------------------------------------------------------------------
// setTOF() -- Set the time of flight
//------------------------------------------------------------------------------
void IWeapon::setTOF(const double newTOF)
{
   tof = newTOF;
}

//------------------------------------------------------------------------------
// weaponGuidance() -- default guidance
//------------------------------------------------------------------------------
void IWeapon::weaponGuidance(const double)
{
}

//------------------------------------------------------------------------------
// weaponDynamics -- default dynamics
//------------------------------------------------------------------------------
void IWeapon::weaponDynamics(const double)
{
}

//------------------------------------------------------------------------------
// updateTOF -- default time of flight
//------------------------------------------------------------------------------
void IWeapon::updateTOF(const double dt)
{
   // As long as we're active ...
   if (isMode(Mode::ACTIVE)) {

      // update time of flight,
      setTOF( getTOF() + dt );

      // and check for the end of the flight
      if (getTOF() >= getMaxTOF()) {
         setMode(Mode::DETONATED);
         setDetonationResults( Detonation::DETONATION );

         BEGIN_RECORD_DATA_SAMPLE( getWorldModel()->getDataRecorder(), REID_WEAPON_DETONATION )
            SAMPLE_3_OBJECTS( this, getLaunchVehicle(), getTargetPlayer() )
            SAMPLE_2_VALUES( static_cast<int>(Detonation::DETONATION), 0.0 )
         END_RECORD_DATA_SAMPLE()

         return;
      }
   }
}


//------------------------------------------------------------------------------
// positionTracking() -- update target position from target player position
//------------------------------------------------------------------------------
void IWeapon::positionTracking()
{
    if (posTrkEnb) {

        // When we have track manager -- follow the first track
        if (tgtTrack != nullptr) {
            setTargetPosition(tgtTrack->getPosition());
            setTargetVelocity(tgtTrack->getVelocity());
        } else if (tgtPlayer != nullptr) {
            // No sensor, but we have a target player -- fake it and just follow the target
            base::Vec3d p0{getPosition()};
            base::Vec3d vel{getVelocity()};
            setTargetPosition(tgtPlayer->getPosition() - p0);
            setTargetVelocity(tgtPlayer->getVelocity() - vel);
        } else {
            // Loss of position tracking ...
            posTrkEnb = false;
        }

    }
}

//------------------------------------------------------------------------------
// Computes and sets 'loc' to our location relative to the target player, 'tgt'
//------------------------------------------------------------------------------
bool IWeapon::computeTargetLocation(base::Vec3d* const loc, const IPlayer* const tgt)
{
   bool ok{};
   if (tgt != nullptr && loc != nullptr) {
      base::Vec3d posP = getPosition() - tgt->getPosition();
      base::Vec3d posB = tgt->getRotMat() * posP;
      *loc = posB;
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Compute the location of the detonation relative to the target player
//------------------------------------------------------------------------------
bool IWeapon::setLocationOfDetonation()
{
   bool ok{};

   // Find our target (if any)
   const IPlayer* tgt{getTargetPlayer()};
   if (tgt == nullptr) {
      const ITrack* trk{getTargetTrack()};
      if (trk != nullptr) tgt = trk->getTarget();
   }

   // computer the location of the detonation relative to the target player
   if (tgt != nullptr) {
      base::Vec3d loc;
      ok = computeTargetLocation(&loc, tgt);
      if (ok) setDetonationLocation(loc);
   }
   return ok;
}

//------------------------------------------------------------------------------
// Standard get routines
//------------------------------------------------------------------------------

// Returns pre-ref()'d pointer to the initial or fly-out based on modes
IWeapon* IWeapon::getPointer()
{
   if (flyoutWpn != nullptr) {
      return flyoutWpn.getRefPtr();
   } else {
      this->ref();
      return this;
   }
}

// Returns pre-ref()'d pointer to the initial or fly-out based on modes (const version)
const IWeapon* IWeapon::getPointer() const
{
   if (flyoutWpn != nullptr) {
      return flyoutWpn.getRefPtr();
   } else {
      this->ref();
      return this;
   }
}

// True if weapon type IDs match
bool IWeapon::isWeaponID(const int n) const
{
   return (getWeaponID() == n);
}

// Weapon type ID number
int IWeapon::getWeaponID() const
{
   return weaponID;
}

// Returns true if the weapon is a member of the test category
bool IWeapon::isCategory(const int testCategory) const
{
   return (testCategory & getCategory()) != 0;
}

// Our launcher, if any
IStores* IWeapon::getLauncher()
{
   return launcher;
}

// Our launcher, if any (const version)
const IStores* IWeapon::getLauncher() const
{
   return launcher;
}

// Station index (number)
unsigned int IWeapon::getStation() const
{
   return station;
}

// True if  the weapon has been released
bool IWeapon::isReleased() const
{
   return released;
}

// Weapon power flag
bool IWeapon::isPowerOn() const
{
   return power;
}

// Blocked weapon flag (can not be released if true)
bool IWeapon::isBlocked() const
{
   return blocked;
}

// True if the weapon can be jettisioned
bool IWeapon::isJettisonable() const
{
   return canJettison;
}

// True if the weapon has been jettisioned
bool IWeapon::isJettisoned() const
{
   return jettisoned;
}

// True if the weapon has failed
bool IWeapon::isFailed() const
{
   return failed;
}

// True if the weapon is hung
bool IWeapon::isHung() const
{
   return hung;
}

// True if the weapon will hang on release
bool IWeapon::getWillHang() const
{
   return willHang;
}

// True if this is a dummy weapon (someone else with fly it out)
bool IWeapon::isDummy() const
{
   return dummyFlg;
}

// Time Of Flight (seconds) since release
double IWeapon::getTOF() const
{
   return tof;
}

// Max TOF (seconds)
double IWeapon::getMaxTOF() const
{
   return maxTOF;
}

// Time-to-Start guidance (seconds since release)
double IWeapon::getTSG() const
{
   return tsg;
}

// Start-Of-Burn time (seconds since release)
double IWeapon::getSOBT() const
{
   return sobt;
}

// End-Of-Burn time (seconds since release)
double IWeapon::getEOBT() const
{
   return eobt;
}

// is guidance system enabled (default check)
bool IWeapon::isGuidanceEnabled() const
{
   return (getTOF() >= tsg) && ((getCategory() & GUIDED) != 0) && isTargetPositionValid();
}

// Weapon engine (rocket) on
bool IWeapon::isEngineBurnEnabled() const
{
   return (tof >= sobt && tof <= eobt);
}

// Max burst range (meters) -- most players will be damaged within this range
double IWeapon::getMaxBurstRng() const
{
   return maxBurstRng;
}

// Lethal range (meters) -- most players will be killed within this range
double IWeapon::getLethalRange() const
{
   return lethalRange;
}

// Max gimbal angle (radians)
double IWeapon::getMaxGimbalAngle() const
{
   return maxGimbal;
}

// Pointer to the player that launched us
IPlayer* IWeapon::getLaunchVehicle()
{
   return launchVehicle;
}

// Pointer to the player that launched us (const version)
const IPlayer* IWeapon::getLaunchVehicle() const
{
   return launchVehicle;
}

// True if we have the target position and is it valid
bool IWeapon::isTargetPositionValid() const
{
   return tgtPosValid;
}

// Returns the target position (meters -- NED from simulation ref point)
const base::Vec3d& IWeapon::getTargetPosition() const
{
   return tgtPos;
}

// Our target track, if any
ITrack* IWeapon::getTargetTrack()
{
   return tgtTrack;
}

// Our target track, if any (const version)
const ITrack* IWeapon::getTargetTrack() const
{
   return tgtTrack;
}

// Our target player, if any
IPlayer* IWeapon::getTargetPlayer()
{
   return tgtPlayer;
}

// Our target player, if any (const version)
const IPlayer* IWeapon::getTargetPlayer() const
{
   return tgtPlayer;
}

// Pre-ref()'d pointer to the fly-out weapon
IWeapon* IWeapon::getFlyoutWeapon()
{
   return flyoutWpn.getRefPtr();
}

// Pre-ref()'d pointer to the fly-out weapon (const version)
const IWeapon* IWeapon::getFlyoutWeapon() const
{
   return flyoutWpn.getRefPtr();
}

// Pre-ref()'d pointer to the initial weapon
IWeapon* IWeapon::getInitialWeapon()
{
   return initialWpn.getRefPtr();
}

// Pre-ref()'d pointer to the initial weapon (const version)
const IWeapon* IWeapon::getInitialWeapon() const
{
   return initialWpn.getRefPtr();
}

// Release event ID (to help match weapon launch and detonation events)
unsigned short IWeapon::getReleaseEventID() const
{
   return eventID;
}

// Is weapon is holding in PRE_RELEASE mode?
bool IWeapon::isReleaseHold() const
{
   return releaseHold;
}

// Detonation result code (see 'Detonation' enum)
IWeapon::Detonation IWeapon::getDetonationResults() const
{
   return results;
}

// Range to target at detonation (meters)
double IWeapon::getDetonationRange() const
{
   return detonationRange;
}

// Location of detonation in target player's coord (meters)
const base::Vec3d& IWeapon::getDetonationLocation() const
{
   return tgtDetLoc;
}

//------------------------------------------------------------------------------
// Standard set routines
//------------------------------------------------------------------------------

// setTargetPlayer() -- sets a pointer to the target player
bool IWeapon::setTargetPlayer(IPlayer* const tgt, const bool pt)
{
    tgtPlayer = tgt;
    tgtTrack = nullptr;

    // Track position?
    posTrkEnb = (pt && tgt != nullptr);
    positionTracking();
    return true;
}

// setTargetTrack() -- sets a pointer to the target track
bool IWeapon::setTargetTrack(ITrack* const trk, const bool pt)
{
    tgtPlayer = nullptr;
    tgtTrack = trk;

    // Track position?
    posTrkEnb = (pt && trk != nullptr);
    positionTracking();
    return true;
}

// setTargetPosition() -- set target position -- platform coord (NED)
bool IWeapon::setTargetPosition(const base::Vec3d& newTgtPos)
{
    tgtPos = newTgtPos;
    setTargetPositionValid(true);
    return true;
}

// setTargetPosition() -- set target velocity
bool IWeapon::setTargetVelocity(const base::Vec3d& newTgtVel)
{
   tgtVel = newTgtVel;
   return true;
}

// Sets the target position valid flag
bool IWeapon::setTargetPositionValid(const bool b)
{
   tgtPosValid = b;
   return true;
}

// setLaunchVehicle() -- sets a pointer to the launching player
bool IWeapon::setLaunchVehicle(IPlayer* const lch)
{
    launchVehicle = lch;
    return true;
}

// Sets a weapon type player to release hold mode
bool IWeapon::setReleaseHold(const bool f)
{
    releaseHold = f;
    return true;
}

// Sets the weapon jettisoned flag
bool IWeapon::setJettisoned(const bool f)
{
   jettisoned = f;
   return true;
}

// setFlyoutWeapon() -- sets a pointer to the "fly-out" weapon player
bool IWeapon::setFlyoutWeapon(IWeapon* const p)
{
    flyoutWpn = p;
    return true;
}

// setInitialWeapon() -- sets a pointer to the "initial" weapon
bool IWeapon::setInitialWeapon(IWeapon* const p)
{
    initialWpn = p;
    return true;
}

// setMaxTOF() -- Set max Time-Of-Flight (seconds)
bool IWeapon::setMaxTOF(const double v)
{
    maxTOF =  v;
    return true;
}

// setTSG() -- Set Time-to-Start-Guidance (seconds)
bool IWeapon::setTSG(const double v)
{
    tsg = v;
    return true;
}

// Sets the detonation result code
bool IWeapon::setDetonationResults(const Detonation dr)
{
   results = dr;
   return true;
}

// Sets the detonation location in target player's coord (meters)
bool IWeapon::setDetonationLocation(const base::Vec3d& loc)
{
   tgtDetLoc = loc;
   detonationRange = loc.length();
   return true;
}

// setMaxGimbalAngle() -- Set max gimbal angle (radians)
bool IWeapon::setMaxGimbalAngle(const double v)
{
    maxGimbal =  v;
    return true;
}

// setMaxBurstRng() -- Set max Burst Range (meters)
bool IWeapon::setMaxBurstRng(const double v)
{
    maxBurstRng =  v;
    return true;
}

// setLethalRange() -- Set max kill Range (meters)
bool IWeapon::setLethalRange(const double v)
{
    lethalRange =  v;
    return true;
}

// setSOBT() -- Set Start-Of-Burn-Time (seconds)
bool IWeapon::setSOBT(const double v)
{
    sobt =  v;
    return true;
}

// setEOBT() -- Set End-Of-Burn-Time (seconds)
bool IWeapon::setEOBT(const double v)
{
   eobt =  v;
   return true;
}

// Sets the weapon's type ID number
bool IWeapon::setWeaponID(const int n)
{
   weaponID = n;
   return true;
}

// Sets the release event ID
bool IWeapon::setReleaseEventID(const unsigned short n)
{
   eventID = n;
   return true;
}


// Sets our launcher and station number
bool IWeapon::setLauncher(IStores* const l, const unsigned int s)
{
   launcher = l;
   station = s;
   return true;
}

// Sets the weapon released flag
bool IWeapon::setReleased(const bool f)
{
   released = f;
   return true;
}

// Sets the weapon power flag
bool IWeapon::setPower(const bool f)
{
   power = f;
   return true;
}

// Sets the weapon blocked flag
bool IWeapon::setBlocked(const bool b)
{
   blocked = b;
   return true;
}

// Sets the jettision enable flag
bool IWeapon::setJettisonable(const bool f)
{
   canJettison = f;
   return true;
}

// Sets the weapon failed flag
bool IWeapon::setFailed(const bool f)
{
   failed = f;
   return true;
}

// Sets the hung weapon flag
bool IWeapon::setHung(const bool f)
{
   hung = f;
   return true;
}

// Sets the 'will' hang flag
bool IWeapon::setWillHang(const bool f)
{
   willHang = f;
   return true;
}

// Sets the dummy weapon flag
bool IWeapon::setDummy(const bool f)
{
   dummyFlg = f;
   return true;
}

//------------------------------------------------------------------------------
// Set Slot routines --
//------------------------------------------------------------------------------

// released:  Weapon has been released
bool IWeapon::setSlotReleased(const base::Boolean* const p)
{
    setReleased( p->asBool() );
    return true;
}

// failed: Weapon failed (e.g., reasonableness Test)
bool IWeapon::setSlotFailed(const base::Boolean* const p)
{
    setFailed( p->asBool() );
    return true;
}

// Power: weapon power flag
bool IWeapon::setSlotPower(const base::Boolean* const p)
{
    setPower( p->asBool() );
    return true;
}

// hang: Will be a hung store
bool IWeapon::setSlotWillHang(const base::Boolean* const p)
{
    setWillHang( p->asBool() );
    return true;
}

// hung: Hung store
bool IWeapon::setSlotHung(const base::Boolean* const p)
{
    setHung( p->asBool() );
    return true;
}

// dummy: Dummy store
bool IWeapon::setSlotDummy(const base::Boolean* const p)
{
    setDummy( p->asBool() );
    return true;
}

// maxTOF:  max time of flight      (base::Time)
bool IWeapon::setSlotMaxTOF(const base::ITime* const p)
{
   bool ok{};
   if (p != nullptr) {
      ok = setMaxTOF(p->getValueInSeconds());
   }
   return ok;
}

// maxTOF:  max time of flight      (sec)
bool IWeapon::setSlotMaxTOF(const base::INumber* const p)
{
    return setMaxTOF( p->asDouble() );
}

// tsg: time to start guidance
bool IWeapon::setSlotTSG(const base::ITime* const p)
{
   bool ok{};
   if (p != nullptr) {
      ok = setTSG(p->getValueInSeconds());
   }
   return ok;
}

// tsg: time to start guidance    (sec)
bool IWeapon::setSlotTSG(const base::INumber* const p)
{
    return setTSG( p->asDouble() );
}

// maxBurstRng: max burst range
bool IWeapon::setSlotMaxBurstRng(const base::ILength* const p)
{
   bool ok{};
   if (p != nullptr) {
      ok = setMaxBurstRng(p->getValueInMeters());
   }
   return ok;
}

// maxBurstRng: max burst range    (meters)
bool IWeapon::setSlotMaxBurstRng(const base::INumber* const p)
{
    return setMaxBurstRng( p->asDouble() );
}


// lethalRange: lethal range
bool IWeapon::setSlotLethalRange(const base::ILength* const p)
{
   bool ok{};
   if (p != nullptr) {
      ok = setLethalRange(p->getValueInMeters());
   }
   return ok;
}

// lethalRange: lethal range    (meters)
bool IWeapon::setSlotLethalRange(const base::INumber* const p)
{
    return setLethalRange( p->asDouble() );
}

// sobt: start-of-burn time
bool IWeapon::setSlotSOBT(const base::ITime* const p)
{
   bool ok{};
   if (p != nullptr) {
      ok = setSOBT(p->getValueInSeconds());
   }
   return ok;
}

// sobt: start-of-burn time        (sec)
bool IWeapon::setSlotSOBT(const base::INumber* const p)
{
    setSOBT(p->asDouble());
    return true;
}

// eobt: end-of-burn time
bool IWeapon::setSlotEOBT(const base::ITime* const p)
{
   bool ok{};
   if (p != nullptr) {
      ok = setEOBT(p->getValueInSeconds());
   }
   return ok;
}

// eobt: end-of-burn time        (sec)
bool IWeapon::setSlotEOBT(const base::INumber* const p)
{
    setEOBT( p->asDouble() );
    return true;
}

// maxBurstRng: max burst rng    (meters)
bool IWeapon::setSlotMaxGimbal(const base::IAngle* const p)
{
    setMaxGimbalAngle(p->getValueInRadians());
    return true;
}

// tgtPos: TEST
bool IWeapon::setSlotTgtPos(const base::IList* const numList)
{
    bool ok{};
    double values[3]{};
    const std::size_t n{numList->getNumberList(values, 3)};
    if (n == 3) {
      base::Vec3d tp(values[0], values[1], values[2]);
      setTargetPosition(tp);
      ok = true;
    }
    return ok;
}

// weaponID: weapon type ID
bool IWeapon::setSlotWeaponID(const base::Integer* const p)
{
    setWeaponID( p->asInt() );
    return true;
}

// jettisonable: weapon can be jettisoned
bool IWeapon::setSlotJettisonable(const base::Boolean* const p)
{
    setJettisonable( p->asBool() );
    return true;
}

// testTgtName: TEST only: target player name
bool IWeapon::setSlotTestTgtName(const base::String* const p)
{
   tstTgtNam = p;
   return true;
}

}
}
