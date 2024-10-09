
#include "mixr/models/system/CollisionDetect.hpp"
#include "mixr/models/player/Player.hpp"
#include "mixr/models/WorldModel.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/units/angles.hpp"
#include "mixr/base/units/lengths.hpp"
#include "mixr/base/util/str_utils.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(CollisionDetect, "CollisionDetect")

BEGIN_SLOTTABLE(CollisionDetect)
   "collisionRange",       //  1: Collision range (default: 4 Meters)
   "maxPlayers",           //  2: Max number of players of interest (default: 20)
   "playerTypes",          //  3: List of player of interest types (default: all types )
   "maxRange2Players",     //  4: Max range from ownship to players of interest, or zero for all (default: 1.0 NM)
   "maxAngle2Players",     //  5: Max angle off the 'nose' of our ownship to players of interest, or zero for all (default: 0)
   "localOnly",            //  6: Only check for collisions with local players (default: false)
   "useWorldCoordinates",  //  7: Using world (ECEF) coordinate system; else NED on the simulation gaming area (default: true)
   "sendCrashEvents",      //  8: Send 'CRASH_EVENT' events on collisions (default: false)
END_SLOTTABLE(CollisionDetect)

BEGIN_SLOT_MAP(CollisionDetect)
    ON_SLOT( 1,  setSlotCollisionRange,      base::Length)
    ON_SLOT( 2,  setSlotMaxPlayers,          base::Integer)
    ON_SLOT( 3,  setSlotPlayerTypes,         base::PairStream)
    ON_SLOT( 4,  setSlotMaxRange2Players,    base::Length)
    ON_SLOT( 5,  setSlotMaxAngle2Players,    base::Angle)
    ON_SLOT( 6,  setSlotLocalOnly,           base::Boolean)
    ON_SLOT( 7,  setSlotUseWorldCoordinates, base::Boolean)
    ON_SLOT( 8,  setSlotSendCrashEvents,     base::Boolean)
END_SLOT_MAP()

CollisionDetect::CollisionDetect()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void CollisionDetect::initData()
{
   resizePoiList(20);         // Default: 20 POI
}

void CollisionDetect::copyData(const CollisionDetect& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   playerTypes = org.playerTypes;
   maxRange2Players = org.maxRange2Players;
   maxAngle2Players = org.maxAngle2Players;
   collisionRange = org.collisionRange;
   useWorld = org.useWorld;
   localOnly = org.localOnly;
   sendCrashEvents = org.sendCrashEvents;

   resizePoiList(org.maxPlayers);
}


void CollisionDetect::deleteData()
{
   resizePoiList(0);
}

//------------------------------------------------------------------------------
// Returns the current number of collisions, or zero if none
// -- Pointers to the players that we've collided with and the distances are
//    stored in the caller provided arrays 'players' and 'distances' arrays.
//------------------------------------------------------------------------------
unsigned int CollisionDetect::getCollisions(Player* list[], double distances[], const unsigned int arraySize)
{
   unsigned int n{};
   base::lock(poiLock);
   for (unsigned int i = 0; i < maxPlayers && n < arraySize; i++) {
      if (players[i].active &&  players[i].collided) {
         list[n] = players[i].player.getRefPtr();
         distances[n] = players[i].distance;
         n++;
      }
   }
   base::unlock(poiLock);
   return n;
}

//------------------------------------------------------------------------------
// Set Functions
//------------------------------------------------------------------------------
bool CollisionDetect::setCollisionRange(const double meters)
{
   collisionRange = meters;
   return true;
}

bool CollisionDetect::setMaxRange2Players(const double meters)
{
   maxRange2Players = meters;
   return true;
}

bool CollisionDetect::setMaxAngle2Players(const double radians)
{
   maxAngle2Players = radians;
   return true;
}

bool CollisionDetect::setPlayerTypes(const unsigned int mask)
{
   playerTypes = mask;
   return true;
}

bool CollisionDetect::setMaxPlayers(const unsigned int n)
{
   resizePoiList(n);
   return true;
}

bool CollisionDetect::setUseWorld(const bool flg)
{
   useWorld = flg;
   return true;
}

bool CollisionDetect::setLocalOnly(const bool flg)
{
   localOnly = flg;
   return true;
}

bool CollisionDetect::setSendCrashEventsEnabled(const bool flg)
{
   sendCrashEvents = flg;
   return true;
}


//------------------------------------------------------------------------------
// updateData() -- Filter the simulation's player list to a sub-list of players
// of interest, as defined by slot parameters.
//------------------------------------------------------------------------------
void CollisionDetect::updateData(const double dt)
{
   // Our base class methods
   BaseClass::updateData(dt);

   const Player* const ownship{getOwnship()};
   WorldModel* const sim{getWorldModel()};

   // early out checks ...
   if (ownship == nullptr || sim == nullptr || maxPlayers == 0) return;

   // World (ECEF) to local (NED)
   const base::Matrixd wm{ownship->getWorldMat()};

   // Local (NED) to body coordinates
   const base::Matrixd rm{ownship->getRotMat()};

   // We will be using world (ECEF) coordinates when the 'useWorld'
   // flag is true or when our ownship's local gaming area position
   // is not valid.
   const bool usingEcefFlg{useWorld || !(ownship->isPositionVectorValid())};

   // Position Vector (ECEF or local gaming area NED)
   base::Vec3d ownPos;
   if (usingEcefFlg) {
      ownPos = ownship->getGeocPosition();
   } else {
      ownPos = ownship->getPosition();
   }

   // Cosine of our max FOV angle
   const double cosMaxFovAngle{std::cos(maxAngle2Players)};

   // Mark all active POIs as unmatched ...
   for (unsigned int i = 0; i < maxPlayers; i++) {
      if (players[i].active) players[i].unmatched = true;
   }

   // ---
   // Scan the player list ---
   // ---
   base::PairStream* plist{sim->getPlayers()};
   if (plist != nullptr) {

      base::List::Item* item{plist->getFirstItem()};
      bool finished{};
      while ( item != nullptr && !finished ) {

         // Get the pointer to the target player
         base::Pair* pair{static_cast<base::Pair*>(item->getValue())};
         Player* target{static_cast<Player*>(pair->object())};

         // Did we complete the local only players?
         finished = localOnly && target->isProxyPlayer();

         // We should process this target if ...
         bool processTgt {
            !finished &&                                        // we're not finished AND
            target != ownship &&                                // its not our ownship AND
            target->isActive() &&                               // the target is active AND
            target->isMajorType(playerTypes) &&                 // the target is one of the selected types AND
            (usingEcefFlg || target->isPositionVectorValid())}; // we're using ECEF or the target's gaming area position is valid

         if ( processTgt ) {

            // Target position vector (ECEF or local gaming area NED)
            base::Vec3d tgtPos;
            if (usingEcefFlg) {
               tgtPos = target->getGeocPosition();
            } else {
               tgtPos = target->getPosition();
            }

            // Target Line-Of-Sight (LOS) vector
            base::Vec3d los{tgtPos - ownPos};

            // Normalized and compute length:
            const double range{los.normalize()};

            // In-range check; but only if max range is greater than zero
            bool inRange{maxRange2Players == 0.0};
            if ( !inRange ) {
               inRange = range <= maxRange2Players;
            }

            if (inRange) {

               // Field of View (FOV) check; but only if the max FOV angle is
               // greater than zero
               bool inFov{maxAngle2Players == 0.0};
               if ( !inFov ) {

                  // Transform the LOS vector to local tangent plane NED
                  base::Vec3d losNED{los};
                  if (usingEcefFlg) {
                     // LOS vector: ECEF to NED
                     losNED = wm * los;
                  }

                  // Transform the LOS vector from NED to body coordinates
                  base::Vec3d losBody{rm * losNED};

                  // It's within our max FOV angle when the X component is
                  // greater than the cosine of the max FOV angle.
                  inFov = (losBody.x() >= cosMaxFovAngle);
               }

               if (inFov) {
                  // If we are here then we have a target player that's active,
                  // the correct type, in-range and within our max FOV ...
                  // so update our POI list with it.
                  updatePoiList(target);
               }
            }
         }

         // Next player ...
         item = item->getNext();
      }

      // Unref the player list
      plist->unref();
   }

   // ---
   // Clear all of the unmatched POIs
   // ---
   base::lock(poiLock);
   for (unsigned int i = 0; i < maxPlayers; i++) {
      if ( players[i].active && players[i].unmatched ) {
         players[i].clear();
      }
   }
   base::unlock(poiLock);

   //std::cout << "POI: ";
   //for (unsigned int i = 0; i < maxPlayers; i++) {
   //   if ( players[i].active ) {
   //      const Player* p = players[i].player;
   //      std::cout << *(p->getName()) << "; ";
   //   }
   //}
   //std::cout << std::endl;
}

//------------------------------------------------------------------------------
// System time critical phase callbacks --
//------------------------------------------------------------------------------
void CollisionDetect::process(const double dt)
{
   Player* const ownship{getOwnship()};

   // early out checks ...
   if (ownship == nullptr || maxPlayers == 0 || dt == 0.0) return;

   // We will be using world (ECEF) coordinates when the 'useWorld'
   // flag is true or when our ownship's local gaming area position
   // is not valid.
   const bool usingEcefFlg{useWorld || !(ownship->isPositionVectorValid())};

   // Ownship position and velocity vectors
   //   (ECEF or local gaming area NED)
   base::Vec3d ownPos;
   base::Vec3d ownVel;
   if (usingEcefFlg) {
      ownPos = ownship->getGeocPosition();
      ownVel = ownship->getGeocVelocity();
   } else {
      ownPos = ownship->getPosition();
      ownVel = ownship->getVelocity();
   }

   // Collision range squared
   const double cr2{collisionRange * collisionRange};

   base::lock(poiLock);

   // ---
   // Check all active POIs to see if we've collided
   // ---
   for (unsigned int i = 0; i < maxPlayers; i++) {

      // If this player active  ...
      Player* const tgt{players[i].player};
      if ( players[i].active && tgt != nullptr && tgt->isActive() ) {

         // Target position and velocity vectors
         //   (ECEF or local gaming area NED)
         base::Vec3d tgtPos;
         base::Vec3d tgtVel;
         if (usingEcefFlg) {
            tgtPos = tgt->getGeocPosition();
            tgtVel = tgt->getGeocVelocity();
         } else {
            tgtPos = tgt->getPosition();
            tgtVel = tgt->getVelocity();
         }

         // Ownship to target Line-Of-Sight (LOS) vector
         base::Vec3d los{tgtPos - ownPos};

         // Current and previous target range
         const double tgtRng{los.length()};
         const double tgtRng0{players[i].range};

         // Current and previous ranging rate
         const double rngRate{(tgtRng - tgtRng0)/dt};
         const double rngRate0{players[i].rangeRate};

         // Distance at possible collision
         double distance{tgtRng};

         // Simple check: are we within the collision range?
         bool collision{tgtRng <= collisionRange};

         // Otherwise, if our range was decreasing, but now it's increasing
         // then compute the point of closest approach and see if it is less
         // than the collision range. (need at less two passes to get valid
         // previous range and rate values)
         if ( !collision && rngRate0 < 0 && rngRate >= 0 && players[i].passCnt >= 2) {

            // Compute the relative velocity vector.
            base::Vec3d velRel{tgtVel - ownVel};

            // Relative velocity squared,
            double velRelSq{velRel.length2()};
            if (velRelSq > 0) {

               // LOS vector (dot) relative velocity vector
               const double rdv{los * velRel};

               // Interpolate back to the closest point
               const double ndt{-rdv/velRelSq};
               base::Vec3d closestPoint{los + (velRel*ndt)};

               // Compute the range squared at the closest point
               const double r2{closestPoint.length2()};

               // Did we collide at the closest point?
               if (r2 <= cr2) {
                  collision = true;
                  distance = std::sqrt(r2);
               }
            }
         }

         // Did we have a collision?
         if (collision) {
            players[i].collided = true;
            players[i].distance = distance;
            if ( isSendCrashEventsEnabled() ) {
               tgt->event(CRASH_EVENT, ownship);
               ownship->event(CRASH_EVENT, tgt);
            }
         } else {
            players[i].collided = false;
         }

         // Save the range and rate for the next pass
         players[i].range = tgtRng;
         players[i].rangeRate = rngRate;
         players[i].passCnt++;

      }
   }

   base::unlock(poiLock);

   //{
   //   static const unsigned int SIZE = 10;
   //   Player* list[SIZE];
   //   double distances[SIZE];
   //   unsigned int n = getCollisions(list, distances, SIZE);
   //   for (unsigned int i = 0; i < n; i++) {
   //      std::cout << "collision: " << *(list[i]->getName());
   //      std::cout << "; dist = " << distances[i] << std::endl;
   //      list[i]->unref();
   //   }
   //}
}


//------------------------------------------------------------------------------
// Update the POI list with this target player; that is, check if its a match
// and if not then add it as a new POI.
//------------------------------------------------------------------------------
void CollisionDetect::updatePoiList(Player* const target)
{
   if (maxPlayers > 0 && target != nullptr) {

      base::lock(poiLock);

      // ---
      // First try to match it with an existing POI
      // ---
      bool matched{};
      unsigned int idx{maxPlayers};
      for (unsigned int i = 0; i < maxPlayers && !matched; i++) {
         const Player* p{players[i].player};
         if (players[i].active && p == target) {
            // Matched!
            players[i].unmatched = false;
            matched = true;
         } else if ( !players[i].active ) {
            // Inactive slot -- to be used if we don't get a match
            idx = i;
         }
      }

      // ---
      // If we didn't match an existing POI then add it
      // ---
      if ( !matched && idx < maxPlayers ) {
         players[idx].player = target;
         players[idx].range = 0.0;
         players[idx].rangeRate = 0.0;
         players[idx].collided = false;
         players[idx].distance = 0.0;
         players[idx].unmatched = false;
         players[idx].passCnt = 0;
         players[idx].active = true;
      }

      base::unlock(poiLock);
   }
}


//------------------------------------------------------------------------------
// Resize our POI array
//------------------------------------------------------------------------------
bool CollisionDetect::resizePoiList(const unsigned int newSize)
{
   if (newSize != maxPlayers) {

      // Clear the old list (has its own lcLock())
      clearPoiList();

      base::lock(poiLock);

      // Free the old list memory
      if (maxPlayers > 0 && players != nullptr) {
         delete[] players;
         players = nullptr;
      }

      // Assign the new size
      maxPlayers = newSize;

      // Allocate a new list
      if (maxPlayers > 0) {
         players = new PlayerOfInterest[maxPlayers];
      }

      base::unlock(poiLock);
   }
   return true;
}


//------------------------------------------------------------------------------
// Clear the POI list
//------------------------------------------------------------------------------
void CollisionDetect::clearPoiList()
{
   base::lock(poiLock);
   for (unsigned int i = 0; i < maxPlayers; i++) {
      players[i].clear();
   }
   base::unlock(poiLock);
}


//------------------------------------------------------------------------------
// Slot Functions
//------------------------------------------------------------------------------
bool CollisionDetect::setSlotCollisionRange(const base::Length* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const double meters{msg->getValueInMeters()};
      if (meters >= 0.0) {
         ok = setCollisionRange( meters );
      }
   }
   return ok;
}

bool CollisionDetect::setSlotMaxPlayers(const base::Integer* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int n{msg->asInt()};
      if (n >= 0) {
         ok = setMaxPlayers( static_cast<unsigned int>(n) );
      }
   }
   return ok;
}

bool CollisionDetect::setSlotPlayerTypes(const base::PairStream* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      unsigned int mask{};
      const base::List::Item* item{msg->getFirstItem()};
      while (item != nullptr) {
         const auto pair = static_cast<const base::Pair*>(item->getValue());
         const auto type = dynamic_cast<const base::String*>( pair->object() );
         if (type != nullptr) {
            if ( base::utStrcasecmp(type->c_str(), "air") == 0 ) {
               mask = (mask | Player::AIR_VEHICLE);
            } else if ( base::utStrcasecmp(type->c_str(), "ground") == 0 ) {
               mask = (mask | Player::GROUND_VEHICLE);
            } else if ( base::utStrcasecmp(type->c_str(), "weapon") == 0 ) {
               mask = (mask | Player::WEAPON);
            } else if ( base::utStrcasecmp(type->c_str(), "ship") == 0 ) {
               mask = (mask | Player::SHIP);
            } else if ( base::utStrcasecmp(type->c_str(), "building") == 0 ) {
               mask = (mask | Player::BUILDING);
            } else if ( base::utStrcasecmp(type->c_str(), "lifeform") == 0 ) {
               mask = (mask | Player::LIFE_FORM);
            } else if ( base::utStrcasecmp(type->c_str(), "space") == 0 ) {
               mask = (mask | Player::SPACE_VEHICLE);
            }
         }
         item = item->getNext();
      }
      ok = setPlayerTypes(mask);
   }
   return ok;
}

bool CollisionDetect::setSlotMaxRange2Players(const base::Length* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const double meters{msg->getValueInMeters()};
      if (meters >= 0.0) {
         ok = setMaxRange2Players( meters );
      }
   }
   return ok;
}

bool CollisionDetect::setSlotMaxAngle2Players(const base::Angle* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const double radians{msg->getValueInRadians()};
      if (radians >= 0.0) {
         ok = setMaxAngle2Players( radians );
      }
   }
   return ok;
}

bool CollisionDetect::setSlotUseWorldCoordinates(const base::Boolean* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setUseWorld( msg->asBool() );
   }
   return ok;
}

bool CollisionDetect::setSlotLocalOnly(const base::Boolean* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setLocalOnly( msg->asBool() );
   }
   return ok;
}

bool CollisionDetect::setSlotSendCrashEvents(const base::Boolean* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setSendCrashEventsEnabled( msg->asBool() );
   }
   return ok;
}

}
}
