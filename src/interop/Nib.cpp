
#include "mixr/interop/Nib.hpp"
#include "mixr/interop/Ntm.hpp"

#include "mixr/models/player/air/AirVehicle.hpp"
#include "mixr/models/player/ground/Artillery.hpp"
#include "mixr/models/player/ground/SamVehicle.hpp"
#include "mixr/models/player/weapon/Missile.hpp"
#include "mixr/models/system/StoresMgr.hpp"
#include "mixr/models/SynchronizedState.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"

#include "mixr/base/util/nav_utils.hpp"
#include "mixr/base/util/str_utils.hpp"

#include <cmath>
#include <string>

namespace mixr {
namespace interop {

IMPLEMENT_SUBCLASS(Nib, "Nib")
EMPTY_SLOTTABLE(Nib)

Nib::Nib(const NetIO::IoType t) : ioType(t)
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void Nib::initData()
{
   pname = "MIXR";
   side = models::Player::BLUE;
   mode = models::Player::Mode::INACTIVE;

   drP0.set(0,0,0);
   drV0.set(0,0,0);
   drA0.set(0,0,0);
   drRPY0.set(0,0,0);
   drAV0.set(0,0,0);
   drR0.identity();
   drWwT.identity();
   drOmega.identity();

   drPos.set(0,0,0);
   drAngles.set(0,0,0);

   smoothVel.set(0,0,0);
}

void Nib::copyData(const Nib& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   ioType = org.ioType;

   const models::Player* p{org.pPlayer};
   setPlayer( const_cast<models::Player*>(p) );
   setNetIO(nullptr);
   setTypeMapper(org.ntm);

   checked = org.checked;
   entityTypeChecked = org.entityTypeChecked;

   playerID = org.playerID;

   pname = org.pname;
   side = org.side;
   mode = org.mode;

   timeoutEnbFlg = org.timeoutEnbFlg;
   execTime = org.execTime;
   utcTime = org.utcTime;
   drNum = org.drNum;
   damage = org.damage;
   smoking = org.smoking;
   flames = org.flames;
   camouflage = org.camouflage;
   detMsgSent = org.detMsgSent;

   execTime = org.execTime;
   utcTime = org.utcTime;

   drNum = org.drNum;
   drP0 = org.drP0;
   drV0 = org.drV0;
   drA0 = org.drA0;
   drRPY0 = org.drRPY0;
   drAV0 = org.drAV0;
   drR0 = org.drR0;
   drWwT = org.drWwT;
   drOmega = org.drOmega;

   drTime = org.drTime;
   drPos = org.drPos;
   drAngles = org.drAngles;

   smoothVel = org.smoothVel;
   smoothTime = org.smoothTime;

   apartWingSweepCnt = org.apartWingSweepCnt;
   apartGearPosCnt = org.apartGearPosCnt;
   apartBayDoorCnt = org.apartBayDoorCnt;
   apartLnchrElevCnt = org.apartLnchrElevCnt;
   apartWingSweep = org.apartWingSweep;
   apartLandingGear = org.apartLandingGear;
   apartBayDoor = org.apartBayDoor;
   apartLnchrElev = org.apartLnchrElev;

   // Need to clear attached missiles -- after clone these need to be found again
   for (int i{}; i < MAX_AMSL; i++) {
      if (apartMsl[i] != nullptr) {
         apartMsl[i]->unref();
         apartMsl[i] = nullptr;
         apartMslCnt[i] = 0;
         apartMslAttached[i] = false;
      }
   }
   apartNumMissiles = 0;

   entityTypeChecked = org.entityTypeChecked;
}


void Nib::deleteData()
{
   setPlayer(nullptr);
   setNetIO(nullptr);
   setTypeMapper(nullptr);

   // Clear attached missiles
   for (int i{}; i < MAX_AMSL; i++) {
      if (apartMsl[i] != nullptr) {
         apartMsl[i]->unref();
         apartMsl[i] = nullptr;
         apartMslCnt[i] = 0;
         apartMslAttached[i] = false;
      }
   }
   apartNumMissiles = 0;
}

//------------------------------------------------------------------------------
// shutdownNotification() -- Shutdown the simulation
//------------------------------------------------------------------------------
bool Nib::shutdownNotification()
{
    setPlayer(nullptr);
    setNetIO(nullptr);
    setTypeMapper(nullptr);

    // Clear attached missiles
   for (int i{}; i < MAX_AMSL; i++) {
      if (apartMsl[i] != nullptr) {
         apartMsl[i]->unref();
         apartMsl[i] = nullptr;
         apartMslCnt[i] = 0;
         apartMslAttached[i] = false;
      }
   }
   apartNumMissiles = 0;

   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// setPlayer() -- sets a pointer to a player
//------------------------------------------------------------------------------
bool Nib::setPlayer(models::Player* const p)
{
    pPlayer = p;
    if (pPlayer != nullptr) {
        playerID = pPlayer->getID();
    } else {
        playerID = 0;
    }
    return true;
}

//------------------------------------------------------------------------------
// return federate name
//------------------------------------------------------------------------------
const std::string& Nib::getFederateName() const
{
   return federateName;
}

//------------------------------------------------------------------------------
// Sets our federate name
//------------------------------------------------------------------------------
bool Nib::setFederateName(const std::string& x)
{
   federateName = x;
   return true;
}

//------------------------------------------------------------------------------
// entityStateManager() -- default entity state manager
//------------------------------------------------------------------------------
bool Nib::entityStateManager(const double)
{
   return true;
}

//------------------------------------------------------------------------------
// weaponFireMsgFactory() -- default weapon fire message factory
//------------------------------------------------------------------------------
bool Nib::weaponFireMsgFactory(const double)
{
   return true;
}

//------------------------------------------------------------------------------
// munitionDetonationMsgFactory() -- default munition detonation message factory
//------------------------------------------------------------------------------
bool Nib::munitionDetonationMsgFactory(const double)
{
   return true;
}

//------------------------------------------------------------------------------
// networkOutputManagers() --  default networkOutputManagers()
//------------------------------------------------------------------------------
bool Nib::networkOutputManagers(const double)
{
    return true;
}

//------------------------------------------------------------------------------
// setOutputPlayerType() -- sets the kind, country, ... variables
//------------------------------------------------------------------------------
bool Nib::setOutputPlayerType(const models::Player* const p)
{
   bool ok{};

   if (getNetIO() != nullptr) {

      // Mark that we've been here.
      setEntityTypeChecked( true );

      // Find "by player" on the output list
      const Ntm* typeMapper{getNetIO()->findNetworkTypeMapper(p)};

      // If we found a type mapper for this Player type,
      // then set the mapper and copy the unique type codes
      if (typeMapper != nullptr) {
         ok = typeMapper->copyEntityType(this);
         if (ok) setTypeMapper(typeMapper);
      }
   }

   return ok;
}


//------------------------------------------------------------------------------
// setTypeMapper() -- sets a pointer to the entity type mapper (Ntm) object
//------------------------------------------------------------------------------
bool Nib::setTypeMapper(const Ntm* const p)
{
    ntm = p;
    return true;
}

//------------------------------------------------------------------------------
// setNetIO() -- sets a pointer to the network model
//------------------------------------------------------------------------------
bool Nib::setNetIO(NetIO* const p)
{
    pNetIO = p;
    return true;
}

//------------------------------------------------------------------------------
// Player data set functions
//------------------------------------------------------------------------------

void Nib::setTimeExec(const double t)
{
    execTime = t;
}

void Nib::setTimeUtc(const double t)
{
    utcTime = t;
}

void Nib::setPlayerID(const unsigned short v)
{
    playerID = v;
}

void Nib::setMode(const models::Player::Mode m)
{
    mode = m;
}

void Nib::setSide(const models::Player::Side s)
{
    side = s;
}

void Nib::setEntityTypeChecked(const bool f)
{
    entityTypeChecked = f;
}

// Sets the damage for this player
bool Nib::setDamage(const double v)
{
   double x{v};
   if (x < 0) x = 0.0;
   if (x > 1) x = 1.0;
   damage = x;
   return true;
}

// Sets the smoke for this player
bool Nib::setSmoke(const double v)
{
   double x{v};
   if (x < 0) x = 0.0;
   if (x > 1) x = 1.0;
   smoking = x;
   return true;
}

// Sets the flames for this player
bool Nib::setFlames(const double v)
{
   double x{v};
   if (x < 0) x = 0.0;
   if (x > 1) x = 1.0;
   flames = x;
   return true;
}

// Sets the camouflage type for this player
bool Nib::setCamouflageType(const unsigned int v)
{
   camouflage = v;
   return true;
}

// Sets the detonation message sent flag
bool Nib::setDetonationMessageSent(const bool flg)
{
   detMsgSent = flg;
   return true;
}


//------------------------------------------------------------------------------
// isPlayerStateUpdateRequired() -- check to see if an update is required
//------------------------------------------------------------------------------
bool Nib::isPlayerStateUpdateRequired(const double curExecTime)
{
   enum { NO, YES, UNSURE } result = UNSURE;

   // ---
   // 1) Make sure that we have a valid player and entity type
   // ---
   const models::Player* player{getPlayer()};
   if (player == nullptr || isEntityTypeInvalid()) result = NO;

   // ---
   // 2) Mode changes
   // ---
   if ( (result == UNSURE) && isNotMode( player->getMode()) ) result = YES;

   // 2-a) NIB is being deleted, send one more update to deactivate the entity
   if ( (result == UNSURE) && isMode( models::Player::Mode::DELETE_REQUEST ) ) result = YES;

   // ---
   // 3) When we're a local player, check for one of the following ...
   // ---
   if ( (result == UNSURE) && player->isLocalPlayer()) {

      //double drTime = curExecTime - getTimeExec();
      models::SynchronizedState playerState{player->getSynchronizedState()};
      const double drTime{static_cast<double>(playerState.getTimeExec()) - getTimeExec()};

      // 3-a) Freeze flag has changed
      if ( (player->isFrozen() && isNotFrozen()) || (!player->isFrozen() && isFrozen()) ) {
         result = YES;
      }

      // 3-b) Max DR timeout
      if (result == UNSURE) {
         if ( drTime >= getNetIO()->getMaxTimeDR(this) ) {
            result = YES;
         }
      }

      // 3-c) Appearance has changed
      if (result == UNSURE &&
         (player->getDamage() != getDamage() ||
         player->getSmoke()  != getSmoke()   ||
         player->getFlames() != getFlames() ||
         player->getCamouflageType() != getCamouflageType() )
         ) {
            result = YES;
      }

      // 3-d) Check dead reckoning errors
      if (result == UNSURE && isNotFrozen()) {

         // Compute our dead reckoned position and angles, which are
         // based on our last packet sent.
         base::Vec3d drPos;
         base::Vec3d drAngles;
         mainDeadReckoning(drTime, &drPos, &drAngles);

         // 3-d-1) Position error
         if (!player->isPositionFrozen() && !player->isAltitudeFrozen()) {

            // max position error (meters)
            const double maxPosErr{getNetIO()->getMaxPositionErr(this)};
            const double maxPosErr2{maxPosErr*maxPosErr};  // squared

            // Check if the length of the position error (squared) is greater
            // than the max error (squared)
            //base::Vec3d ppos = player->getGeocPosition();
            const base::Vec3d ppos{playerState.getGeocPosition()};
            const base::Vec3d errPos{drPos - ppos};
            if (errPos.length2() >= maxPosErr2) {
               result = YES;
            }
         }

         // 3-d-2) Orientation error
         if (result == UNSURE && !player->isAttitudeFrozen()) {

            // max angle error (radians)
            const double maxAngleErr{getNetIO()->getMaxOrientationErr(this)};

            // Compute angular error
            //base::Vec3 errAngles = drAngles - player->getGeocEulerAngles();
            base::Vec3d errAngles{drAngles - playerState.getGeocEulerAngles()};

            // Check if any angle error is greater than the max error
            errAngles[0] = std::fabs( base::angle::aepcdDeg(errAngles[0]) );
            if (errAngles[0] >= maxAngleErr) result = YES;

            errAngles[1] = std::fabs( base::angle::aepcdDeg(errAngles[1]) );
            if (errAngles[1] >= maxAngleErr) result = YES;

            errAngles[2] = std::fabs( base::angle::aepcdDeg(errAngles[2]) );
            if (errAngles[2] >= maxAngleErr) result = YES;
         }
      }
   }

   // ---
   // 4) Check for air vehicle articulated and attached parts (always check this)
   // ---
   if ( player != nullptr && player->isMajorType(models::Player::AIR_VEHICLE) ) {

      const models::AirVehicle* av{static_cast<const models::AirVehicle*>(player)};

      // (4-a) Check wing sweep angle.  We only send out wing sweep as
      // an part if the position is greater than zero or if we've previously been
      // sending the wing sweep (count > 0).
      {
         const double angle{av->getWingSweepAngle()};  //  radians
         if (angle > 0 || apartWingSweepCnt > 0) {
            // Check if the angle has changed.
            if (angle != apartWingSweep) {
               apartWingSweep = angle;
               apartWingSweepCnt++;
               result = YES;
            }
         }
      }

      // (4-b) Check landing gear position.  We only send out gear position as
      // an part if the gear is not up (pos != 0) or if we've previously been
      // sending the gear position (count > 0).
      {
         const double pos{av->getLandingGearPosition()}; // (0% up; 100% down)
         if (pos > 0 || apartGearPosCnt > 0) {
            // Check if the pos has changed.
            if (pos != apartLandingGear) {
               apartLandingGear = pos;
               apartGearPosCnt++;
               result = YES;
            }
         }
      }

      // (4-c) Check bay door position.  We only send out bay door posiiton as
      // an part if the door is not closed (pos != 0) or if we've previously been
      // sending the door position (count > 0).
      {
         const double pos{av->getWeaponBayDoorPosition()}; // % (0% closed; 100% open)
         if (pos > 0 || apartBayDoorCnt > 0) {
            // Check if the pos has changed.
            if (pos != apartBayDoor) {
               apartBayDoor = pos;
               apartBayDoorCnt++;
               result = YES;
            }
         }
      }

   }

   // ---
   // 5) Check for ground vehicle articulated and attached parts (always check this)
   // ---
   if ( player != nullptr && player->isMajorType(models::Player::GROUND_VEHICLE) ) {

      const models::GroundVehicle* gv{static_cast<const models::GroundVehicle*>(player)};

      // (5-a) Send launcher elevation angle and for an attached missile
      //       (on SamVehicles and Artillery only)
      if ( gv->isClassType(typeid(models::SamVehicle)) || gv->isClassType(typeid(models::Artillery)) ) {

         const double angle{gv->getLauncherPosition()};  //  (radians)

         // First pass --
         if (apartLnchrElevCnt == 0) {

            // find all missiles missiles
            const models::StoresMgr* sm{gv->getStoresManagement()};
            if (sm != nullptr) {
               const base::PairStream* stores{sm->getStores()};
               if (stores != nullptr) {
                  const base::List::Item* item{stores->getFirstItem()};
                  while (item != nullptr && apartNumMissiles < MAX_AMSL) {
                     const auto pair = static_cast<const base::Pair*>(item->getValue());
                     if (pair != nullptr) {
                        const auto msl = dynamic_cast<const models::Missile*>( pair->object() );
                        if (msl != nullptr) {
                           // Save the pointer to the missile, set the missile's change count to 1,
                           // and up the missile count
                           msl->ref();
                           apartMsl[apartNumMissiles] = msl;
                           apartMslAttached[apartNumMissiles] = !(msl->isMode(models::Player::Mode::LAUNCHED));
                           apartMslCnt[apartNumMissiles] = 1;
                           apartNumMissiles++;
                        }
                     }
                     item = item->getNext();
                  }
                  stores->unref();
                  stores = nullptr;
               }

            }

            // If we have missile then set the launcher angle
            if (apartNumMissiles > 0) {
               apartLnchrElev = angle;
               apartLnchrElevCnt++;
            }
         }

         // If we have the launcher angle and missiles then check for changes
         if (apartLnchrElevCnt != 0) {

            // Check if the pos has changed
            if (angle != apartLnchrElev) {
               apartLnchrElev = angle;
               apartLnchrElevCnt++;
               result = YES;
            }

            // Check all missiles for change in launched status
            for (unsigned int i{}; i < apartNumMissiles; i++) {
               bool attached = !(apartMsl[i]->isMode(models::Player::Mode::LAUNCHED));
               if (attached != apartMslAttached[i]) {
                  // There's been a change in status
                  apartMslAttached[i] = attached;
                  apartMslCnt[i]++;
               }
            }

         }

      }

   }

   // ---
   // 6) When we're a network player -- Update when the exec time of the
   //    last input (player's NIB) is different that our exec time.
   // ---
   if ( (result == UNSURE) && player->isProxyPlayer() ) {
      const auto playerNib = dynamic_cast<const Nib*>(player->getNib());
      if (playerNib->getTimeExec() != getTimeExec()) {
         result = YES;
      }
   }

   return (result == YES);
}

//------------------------------------------------------------------------------
// playerState2Nib() -- Sets this NIB's player data
//------------------------------------------------------------------------------
void Nib::playerState2Nib()
{
   const models::Player* player{getPlayer()};
   if (player != nullptr) {
      // set player name
      const std::string cname{player->getName()};
      if (cname != "") setPlayerName(cname.c_str());
      else setPlayerName("MIXR");

      freeze( player->isFrozen() );
      if (!isMode(models::Player::Mode::DELETE_REQUEST)) setMode( player->getMode() );
      setDamage( player->getDamage() );
      setSmoke( player->getSmoke() );
      setFlames( player->getFlames() );
      setCamouflageType( player->getCamouflageType() );
      setSide( player->getSide() );

      // Reset our dead reckoning with the current state data from the player
      //resetDeadReckoning(
      //   RVW_DRM,
      //   player->getGeocPosition(),
      //   player->getGeocVelocity(),
      //   player->getGeocAcceleration(),
      //   player->getGeocEulerAngles(),
      //   player->getGeocAngularVelocities()
      //);

      resetDeadReckoning(
         RVW_DRM,
         player->getSynchronizedState().getGeocPosition(),
         player->getSynchronizedState().getGeocVelocity(),
         player->getSynchronizedState().getGeocAcceleration(),
         player->getSynchronizedState().getGeocEulerAngles(),
         player->getSynchronizedState().getAngularVelocities()
      );

      // mark the current times
      //Simulation* sim = getNetIO()->getSimulation();
      //setTimeExec( static_cast<double>(sim->getExecTimeSec()) );
      setTimeExec( static_cast<double>(player->getSynchronizedState().getTimeExec()) );
      //setTimeUtc( static_cast<double>(sim->getSysTimeOfDay()) );
      setTimeUtc( static_cast<double>(player->getSynchronizedState().getTimeUtc()) );

      {
         //base::Vec3d pos = player->getGeocPosition();
         //base::Vec3d vec = player->getGeocVelocity();

         //std::cout << "playerState2Nib(): geoc pos: (";
         //std::cout << pos[0] << ", ";
         //std::cout << pos[1] << ", ";
         //std::cout << pos[2] << ") ";
         //std::cout << "geoc vel: (";
         //std::cout << vec[0] << ", ";
         //std::cout << vec[1] << ", ";
         //std::cout << vec[2] << ") ";
         //std::cout << std::endl;
      }
   }
}

//------------------------------------------------------------------------------
// nib2PlayerState() --
//   Called by our processInputList() to update the state of networked player.
//------------------------------------------------------------------------------
void Nib::nib2PlayerState()
{
   models::Player* player{getPlayer()};
   if (player != nullptr) {

      // Drive modes
      player->freeze( isFrozen() );
      player->setMode( getMode() );

      // Appearance
      player->setDamage( getDamage() );
      player->setSmoke( getSmoke() );
      player->setFlames( getFlames() );
      player->setCamouflageType( getCamouflageType() );

      // proxy player's position and Euler angles are set by the player
      // during its dynamics phase and using our DR functions.
   }
}

//------------------------------------------------------------------------------
// update entity dead reckoning (incoming entities only)
//------------------------------------------------------------------------------
bool Nib::updateDeadReckoning(
      const double dt,
      base::Vec3d* const pNewPos,
      base::Vec3d* const pNewAngles
   )
{
   bool ok{ioType == NetIO::INPUT_NIB};
   if (ok) {
      double time{updateDrTime(dt)};

      // Main Dead Reckoning Function
      mainDeadReckoning( time, &drPos, &drAngles );
      //std::cout << "updateDeadReckoning(): geoc pos(";
      //std::cout << drPos[0] << ", ";
      //std::cout << drPos[1] << ", ";
      //std::cout << drPos[2] << ") ";

      // Smoothing
      if (smoothTime > 0.0) {
         drPos += (smoothVel * smoothTime);
         smoothTime -= dt;
      }
      //base::Vec3d ss = (smoothVel * smoothTime);
      //std::cout << " smooth(";
      //std::cout << ss[0] << ", ";
      //std::cout << ss[1] << ", ";
      //std::cout << ss[2] << ") ";
      //std::cout << std::endl;

      *pNewPos = drPos;
      *pNewAngles = drAngles;
   }
   return ok;
}

//------------------------------------------------------------------------------
// (re)initialize the dead reckoning function
//------------------------------------------------------------------------------
bool Nib::resetDeadReckoning(
      const unsigned char dr,       // Dead-Reckoning algorithm number
      const base::Vec3d& p,         // Position vector @ T0 (meters) (ECEF)
      const base::Vec3d& v,         // Velocity vector @ T0 (m/sec)  (ECEF or body based on DR algorithm)
      const base::Vec3d& a,         // Acceleration vector @ T0 ((m/sec)/sec) (ECEF or body based on DR algorithm)
      const base::Vec3d& rpy,       // Euler angles @ T0 (rad) [ phi theta psi ] (Body/ECEF)
      const base::Vec3d& av,        // Angular rates @ T0 (rad/sec)  [ phi theta psi ] (Body/ECEF)
      const double time             // Initial time
   )
{
   base::Vec3d drPosN1(drPos);

   // ---
   // Set the initial DR values
   // ---
   drP0 = p;
   drV0 = v;
   drA0 = a;
   drRPY0 = rpy;
   drAV0 = av;

   // ---
   // Compute the matrices
   // ---
   drComputeMatrixR0(drRPY0, &drR0);
   drComputeMatrixWwT(drAV0, &drWwT);
   drComputeMatrixOmega(drAV0, &drOmega);

   // ---
   // Update the smoothing values ...
   //    If the new position is less than one KM from the old DR position
   //    then we'll smooth over the next 2 seconds.  Otherwise, smoothing
   //    is turned off and we'll just jump to the new position.
   // ---
   smoothTime = 0; // default is off
   smoothVel.set(0,0,0);
   base::Vec3d err;
   if (ioType == NetIO::INPUT_NIB && drTime > 0) {
      err = drPosN1 - drP0;
      const double len{err.length()};
      if (len < (2.0 * base::length::KM2M) ) {
         smoothVel = err/drTime;
         smoothTime = drTime;
      }
   }

   //double drTimeN1 = drTime;
   drNum = dr;
   drTime = time;

   //if (ioType == NetIO::INPUT_NIB) {
      //std::cout << "resetDeadReckoning(): drTime = " << drTimeN1 << std::endl;
      //std::cout << "drPos(";
      //std::cout << drPos[0] << ", ";
      //std::cout << drPos[1] << ", ";
      //std::cout << drPos[2] << ") ";
      //std::cout << std::endl;
      //std::cout << "drP0(";
      //std::cout << drP0[0] << ", ";
      //std::cout << drP0[1] << ", ";
      //std::cout << drP0[2] << ") ";
      //std::cout << std::endl;
      //std::cout << "err(";
      //std::cout << err[0] << ", ";
      //std::cout << err[1] << ", ";
      //std::cout << err[2] << ") ";
      //std::cout << std::endl;
      //std::cout << "sVel(";
      //std::cout << smoothVel[0] << ", ";
      //std::cout << smoothVel[1] << ", ";
      //std::cout << smoothVel[2] << ") ";
      //std::cout << std::endl;
   //}

   return true;
}

//------------------------------------------------------------------------------
// Main Dead Reckoning Function
//------------------------------------------------------------------------------
bool Nib::mainDeadReckoning(
      const double dT,                 // DR time (seconds)
      base::Vec3d* const pNewP0,       // DR Position vector @ time = 'dT' (meters) (ECEF)
      base::Vec3d* const pNewRPY       // DR Euler angles @ time = 'dT' (rad) [ phi theta psi ] (Body/ECEF)
   ) const
{
   switch (drNum) {

      // World, No rotation, 1st order linear
      case FPW_DRM: {
         *pNewRPY = drRPY0;
         *pNewP0 = drP0 + drV0*dT;
      }
      break;

      // World, 1st order rotation, 1st order linear
      case RPW_DRM: {
         base::Matrixd DR;
         drComputeMatrixDR(dT, drAV0, drWwT, drOmega, &DR);
         base::Matrixd Rwb = DR * drR0;
         base::nav::computeEulerAngles(Rwb, pNewRPY);

         *pNewP0 = drP0 + drV0*dT;
      }
      break;

      // World, 1st order rotation, 2nd order linear
      case RVW_DRM: {
         base::Matrixd DR;
         drComputeMatrixDR(dT, drAV0, drWwT, drOmega, &DR);
         base::Matrixd Rwb = DR * drR0;
         base::nav::computeEulerAngles(Rwb, pNewRPY);

         *pNewP0 = drP0 + drV0*dT + drA0*(0.5*dT*dT);
      }
      break;

      // World, No rotation, 2nd order linear
      case FVW_DRM: {
         *pNewRPY = drRPY0;
         *pNewP0 = drP0 + drV0*dT + drA0*(0.5*dT*dT);
      }
      break;

      // Body, No rotation, 1st order linear
      case FPB_DRM: {
         *pNewRPY = drRPY0;

         base::Matrixd R1;
         drComputeMatrixR1(dT, drAV0, drWwT, drOmega, &R1);

         base::Matrixd Rwb = drR0;
         Rwb.transpose();

         *pNewP0 = drP0 + Rwb * (R1 * drV0);
      }
      break;

      // Body, 1st order rotation, 1st order linear
      case RPB_DRM: {
         base::Matrixd DR;
         drComputeMatrixDR(dT, drAV0, drWwT, drOmega, &DR);
         base::Matrixd Rwb = DR * drR0;

         base::nav::computeEulerAngles(Rwb, pNewRPY);

         base::Matrixd R1;
         drComputeMatrixR1(dT, drAV0, drWwT, drOmega, &R1);

         Rwb.transpose();
         *pNewP0 = drP0 + Rwb * (R1 * drV0);
      }
      break;

      // Body, 1st order rotation, 2nd order linear
      case RVB_DRM: {
         base::Matrixd DR;
         drComputeMatrixDR(dT, drAV0, drWwT, drOmega, &DR);
         base::Matrixd Rwb = DR * drR0;

         base::nav::computeEulerAngles(Rwb, pNewRPY);

         base::Matrixd R1;
         drComputeMatrixR1(dT, drAV0, drWwT, drOmega, &R1);

         base::Matrixd R2;
         drComputeMatrixR2(dT, drAV0, drWwT, drOmega, &R1);

         Rwb.transpose();
         *pNewP0 = drP0 + Rwb * ((R1*drV0) + (R2*drA0));
      }
      break;

      // Body, No rotation, 2nd order linear
      case FVB_DRM: {
         *pNewRPY = drRPY0;

         base::Matrixd R1;
         drComputeMatrixR1(dT, drAV0, drWwT, drOmega, &R1);

         base::Matrixd R2;
         drComputeMatrixR2(dT, drAV0, drWwT, drOmega, &R1);

         base::Matrixd Rwb = drR0;
         Rwb.transpose();

         *pNewP0 = drP0 + Rwb * ((R1*drV0) + (R2*drA0));
      }
      break;

      // No dead reckoning
      case STATIC_DRM: {
         *pNewP0 = drP0;
         *pNewRPY = drRPY0;
      }
      break;

      case OTHER_DRM:   // User defined
      default:          // default
         std::cout << "drNum == default" << std::endl;
         *pNewP0 = drP0;
         *pNewRPY = drRPY0;
      break;
   }

   return true;
}

//------------------------------------------------------------------------------
// drComputeMatrixR0
//------------------------------------------------------------------------------
bool Nib::drComputeMatrixR0(
      const base::Vec3d& RPY,      // [radians]
      base::Matrixd* const pR0     // Rotational matrix R0
   )
{
   //--------------------------------------------------------------------------
   // intermediate values
   //--------------------------------------------------------------------------

   const double Rol{RPY[0]};
   const double Pch{RPY[1]};
   const double Yaw{RPY[2]};

   const double sinRol{std::sin(Rol)};
   const double cosRol{std::cos(Rol)};
   const double sinPch{std::sin(Pch)};
   const double cosPch{std::cos(Pch)};
   const double sinYaw{std::sin(Yaw)};
   const double cosYaw{std::cos(Yaw)};

   //--------------------------------------------------------------------------
   // Compute R0 - initial orientation matrix (World --> Body)
   //--------------------------------------------------------------------------
   (*pR0)(0,0) = cosPch * cosYaw;
   (*pR0)(0,1) = cosPch * sinYaw;
   (*pR0)(0,2) = -sinPch;

   (*pR0)(1,0) = sinRol * sinPch * cosYaw - cosRol * sinYaw;
   (*pR0)(1,1) = sinRol * sinPch * sinYaw + cosRol * cosYaw;
   (*pR0)(1,2) = sinRol * cosPch;

   (*pR0)(2,0) = cosRol * sinPch * cosYaw + sinRol * sinYaw;
   (*pR0)(2,1) = cosRol * sinPch * sinYaw - sinRol * cosYaw;
   (*pR0)(2,2) = cosRol * cosPch;

   return true;
}


//------------------------------------------------------------------------------
// Compute wwT matrix
//------------------------------------------------------------------------------
bool Nib::drComputeMatrixWwT(
      const base::Vec3d& av,       // angular velocities [rps]
      base::Matrixd* const pwwT    // matrix
   )
{
   //--------------------------------------------------------------------------
   // Compute wwT Matrix
   //--------------------------------------------------------------------------

   const double Wx{av[0]};
   const double Wy{av[1]};
   const double Wz{av[2]};

   (*pwwT)(0,0) = Wx * Wx;
   (*pwwT)(0,1) = Wx * Wy;
   (*pwwT)(0,2) = Wx * Wz;

   (*pwwT)(1,0) = Wy * Wx;
   (*pwwT)(1,1) = Wy * Wy;
   (*pwwT)(1,2) = Wy * Wz;

   (*pwwT)(2,0) = Wz * Wx;
   (*pwwT)(2,1) = Wz * Wy;
   (*pwwT)(2,2) = Wz * Wz;

   return true;
}


//------------------------------------------------------------------------------
// Compute the Omega matrix
//------------------------------------------------------------------------------
bool Nib::drComputeMatrixOmega(
      const base::Vec3d& av,       // angular velocities [rps]
      base::Matrixd* const pOmega  // matrix
   )
{
   //--------------------------------------------------------------------------
   // Compute Omega Matrix
   //--------------------------------------------------------------------------

   const double Wx{av[0]};
   const double Wy{av[1]};
   const double Wz{av[2]};

   (*pOmega)(0,0) = 0.0;
   (*pOmega)(0,1) = -Wz;
   (*pOmega)(0,2) = +Wy;

   (*pOmega)(1,0) = +Wz;
   (*pOmega)(1,1) = 0.0;
   (*pOmega)(1,2) = -Wx;

   (*pOmega)(2,0) = -Wy;
   (*pOmega)(2,1) = +Wx;
   (*pOmega)(2,2) = 0.0;

   return true;
}


//------------------------------------------------------------------------------
// drComputeMatrixDR
//------------------------------------------------------------------------------
bool Nib::drComputeMatrixDR(
      const double dT,            // DR time [sec]
      const base::Vec3d& av,      // angular velocities [rps]
      const base::Matrixd& wwT,   // wwT matrix
      const base::Matrixd& omega, // omega matrix
      base::Matrixd* const pDR    // DR matrix
   )
{
   //--------------------------------------------------------------------------
   // intermediate values
   //--------------------------------------------------------------------------

   const double Wx{av[0]};
   const double Wy{av[1]};
   const double Wz{av[2]};
   const double absAV2{Wx*Wx + Wy*Wy + Wz*Wz};

   if (absAV2 > 0.0) {
      const double absAV1{std::sqrt(absAV2)};

      const double cosWT{std::cos(absAV1 * dT)};
      const double sinWT{std::sin(absAV1 * dT)};

      //--------------------------------------------------------------------------
      // Get matrix scaling Coefficients (k1, k2, k3)
      //--------------------------------------------------------------------------
      const double k1{(1.0 - cosWT) / absAV2};
      const double k2{cosWT};
      const double k3{sinWT / absAV1};

      //--------------------------------------------------------------------------
      // Compute Dead Reckoning Matrix (DR)
      //--------------------------------------------------------------------------
      static const base::Matrixd I3;
      *pDR = wwT*k1 + I3*k2 - omega*k3;
   }
   else {
      pDR->identity();
   }

   return true;
}


//------------------------------------------------------------------------------
// drComputeMatrixR1
//------------------------------------------------------------------------------
bool Nib::drComputeMatrixR1(
      const double dT,            // DR time [sec]
      const base::Vec3d& av,      // angular velocities [rps]
      const base::Matrixd& wwT,   // wwT matrix
      const base::Matrixd& omega, // omega matrix
      base::Matrixd* const pR1    // Matrix R1
   )
{
   //--------------------------------------------------------------------------
   // intermediate values
   //--------------------------------------------------------------------------

   const double Wx{av[0]};
   const double Wy{av[1]};
   const double Wz{av[2]};
   const double absAV2{Wx*Wx + Wy*Wy + Wz*Wz};

   if (absAV2 > 0.0) {
      const double absAV1{std::sqrt(absAV2)};
      const double absAV3{absAV2 * absAV1};

      const double cosWT{std::cos(absAV1 * dT)};
      const double sinWT{std::sin(absAV1 * dT)};

      //----------------------------------------------------------------------
      // Get Matrix Coefficients (k1, k2, k3)
      //----------------------------------------------------------------------
      const double k1{(absAV1 * dT - sinWT) / absAV3};
      const double k2{sinWT / absAV1};
      const double k3{(1.0 - cosWT) / absAV2};

      //----------------------------------------------------------------------
      // Compute R1 Matrix
      //----------------------------------------------------------------------
      static const base::Matrixd I3;
      *pR1 = wwT*k1 + I3*k2 + omega*k3;
   } else {
      pR1->identity();
   }

   return true;
}


//------------------------------------------------------------------------------
// drComputeMatrixR2
//------------------------------------------------------------------------------
bool Nib::drComputeMatrixR2(
      const double dT,                 // DR time [sec]
      const base::Vec3d& av,      // angular velocities [rps]
      const base::Matrixd& wwT,   // wwT matrix
      const base::Matrixd& omega, // omega matrix
      base::Matrixd* const pR2    // Matrix R2
   )
{
   //--------------------------------------------------------------------------
   // intermediate values
   //--------------------------------------------------------------------------
   const double Wx{av[0]};
   const double Wy{av[1]};
   const double Wz{av[2]};
   const double absAV2{Wx*Wx + Wy*Wy + Wz*Wz};

   if (absAV2 > 0.0) {
      const double absAV1{std::sqrt(absAV2)};
      const double absAV3{absAV2 * absAV1};
      const double absAV4{absAV1 * absAV3};

      const double cosWT{std::cos(absAV1 * dT)};
      const double sinWT{std::sin(absAV1 * dT)};

      //--------------------------------------------------------------------------
      // Get Matrix Scaling Coefficients (k1, k2, k3)
      //--------------------------------------------------------------------------
      const double k1{(0.5*absAV2*dT*dT - cosWT - absAV1*dT*sinWT + 1.0) / absAV4};
      const double k2{(cosWT + absAV1*dT*sinWT - 1.0) / absAV2};
      const double k3{(sinWT - absAV1*dT*cosWT) / absAV3};

      //--------------------------------------------------------------------------
      // Compute R2 Matrix
      //--------------------------------------------------------------------------
      static const base::Matrixd I3;
      *pR2 = wwT*k1 + I3*k2 + omega*k3;
   } else {
      pR2->identity();
   }

   return true;
}

}
}
