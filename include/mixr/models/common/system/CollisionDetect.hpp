
#ifndef __mixr_models_common_CollisionDetect_HPP__
#define __mixr_models_common_CollisionDetect_HPP__

#include "mixr/models/common/system/System.hpp"
#include "mixr/config.hpp"
#include "mixr/base/units/util/length_utils.hpp"

namespace mixr {
namespace base { class Angle; class Boolean; class Integer; class Length; class Number; class PairStream; }
namespace models {
class Player;

//------------------------------------------------------------------------------
// Class: CollisionDetect
// Description: Collision detection component -- when added as a component to
//              a player (i.e., our ownship), we will look for collisions with
//              other players in the simulation player list.  When a collision
//              is detected, 'CRASH_EVENT' events are sent to our ownship and the
//              player we collided with.
//
// Tasks:
// 1) updateData() -- background thread --
//    Using the 'maxPlayers', 'playerTypes', 'maxRange2Players', 'maxAngle2Players'
//    and 'localOnly' slot parameters, this function filters the players list
//    to create a sublist of players that are checked by the process() function.
//
// 2) process() -- time critical thread --
//    Checks the distance from own ownship to the players in the sublist, which
//    was created by updateData(), and generates 'CRASH_EVENT' events when the
//    distance is less than or equal to 'collisionRange'.
//
// Factory name: CollisionDetect
// Slots:
//    collisionRange      <Length>         ! Collision range (default: 4 Meters)
//    maxPlayers          <Integer>        ! Max number of players of interest (default: 20)
//    playerTypes         <PairStream>     ! List of player of interest types (default: all types )
//                                         !   Valid types: { "air" "ground" "weapon" "ship" "building" "lifeform" "space" }
//    maxRange2Players    <Length>         ! Max range from ownship to players of interest, or zero for all (default: 1.0 NM)
//    maxAngle2Players    <Angle>          ! Max angle off the 'nose' of our ownship to players of interest, or zero for all (default: 0)
//    localOnly           <Boolean>        ! Only check for collisions with local players (default: false)
//    useWorldCoordinates <Boolean>        ! Using world (ECEF) coordinate system; else NED on the simulation gaming area (default: true)
//    sendCrashEvents     <Boolean>        ! Send 'CRASH_EVENT' events on collisions (default: false)
//
//------------------------------------------------------------------------------
class CollisionDetect : public System
{
   DECLARE_SUBCLASS(CollisionDetect, System)

public:
   CollisionDetect();

   // Returns the current number of collisions, or zero if none.
   // -- Pre-ref()'d pointers to the players that we've collided with and the collision
   // distances (meters) are stored in the caller provided arrays 'list' and 'distances'.
   unsigned int getCollisions(Player* list[], double distances[], const unsigned int arraySize);

   double getCollisionRange() const;      // Collision range (meters)
   double getMaxRange2Players() const;    // Max range from ownship to players of interest, or zero for all (meters)
   double getMaxAngle2Players() const;    // Max angle off the 'nose' of our ownship to players of interest, or zero for all (radians)
   unsigned int getPlayerTypes() const;   // Players of interest type mask (see Player::MajorType bits)
   unsigned int getMaxPlayers() const;    // Max number of players of interest
   bool isUsingWorldCoordinates() const;  // True if using world (ECEF) coordinates; else using NED on the simulation gaming area
   bool isLocalOnly() const;              // True if check only local players
   bool isSendCrashEventsEnabled() const; // True if sending crash events is enabled

   virtual bool setCollisionRange(const double meters);
   virtual bool setMaxRange2Players(const double meters);
   virtual bool setMaxAngle2Players(const double radians);
   virtual bool setPlayerTypes(const unsigned int mask);
   virtual bool setMaxPlayers(const unsigned int n);
   virtual bool setUseWorld(const bool flg);
   virtual bool setLocalOnly(const bool flg);
   virtual bool setSendCrashEventsEnabled(const bool flg);

   void updateData(const double dt = 0.0) override;

protected:
   virtual bool resizePoiList(const unsigned int newSize);
   virtual void clearPoiList();

   // Update the POI list with this target player
   virtual void updatePoiList(Player* const target);

   void process(const double dt) override;     // Phase 3

protected:
   class PlayerOfInterest
   {
   public:
      PlayerOfInterest() = default;
      PlayerOfInterest(const PlayerOfInterest&) = delete;
      PlayerOfInterest& operator=(const PlayerOfInterest&) = delete;
      ~PlayerOfInterest() { player = nullptr; }
      
      void clear()        { player = nullptr; active = false; }

      base::safe_ptr<Player> player;    // The player
      double range {};                  // Previous range (m)
      double rangeRate {};              // Previous range rate (m/s)
      double distance {};               // Distance at collision (m)
      unsigned int passCnt {};          // Pass count
      bool active {};                   // This entry is active (in-use)
      bool collided {};                 // We've collided with this player
      bool unmatched {};                // Entry not yet matched during an update
   };

private:
   void initData();

   mutable long poiLock {};      // Semaphore to protect the POI list

   unsigned int playerTypes {0xFFFF};  // Player types mask (default: all types)
   double maxRange2Players {1.0 * base::length::NM2M};   // Max range from ownship to players of interest, or zero for all (meters) (default: 1.0 NM)
   double maxAngle2Players {};         // Max angle off the 'nose' of our ownship to players of interest, or zero for all (radians)
   double collisionRange {4.0};        // Collision range (meters)
   bool useWorld {true};               // Using player of interest's world coordinates
   bool localOnly {};                  // Local players only
   bool sendCrashEvents {};            // Send crash events

   PlayerOfInterest* players {};       // Player of interest (POI) list
   unsigned int maxPlayers {};         // Max number of players of interest

private:
   // slot table helper methods
   bool setSlotCollisionRange(const base::Length* const);
   bool setSlotMaxPlayers(const base::Integer* const);
   bool setSlotPlayerTypes(const base::PairStream* const);
   bool setSlotMaxRange2Players(const base::Length* const);
   bool setSlotMaxAngle2Players(const base::Angle* const);
   bool setSlotUseWorldCoordinates(const base::Boolean* const);
   bool setSlotLocalOnly(const base::Boolean* const);
   bool setSlotSendCrashEvents(const base::Boolean* const);
};

inline double CollisionDetect::getCollisionRange() const       { return collisionRange; }
inline double CollisionDetect::getMaxRange2Players() const     { return maxRange2Players; }
inline double CollisionDetect::getMaxAngle2Players() const     { return maxAngle2Players; }
inline unsigned int CollisionDetect::getPlayerTypes() const    { return playerTypes; }
inline unsigned int CollisionDetect::getMaxPlayers() const     { return maxPlayers; }
inline bool CollisionDetect::isUsingWorldCoordinates() const   { return useWorld; }
inline bool CollisionDetect::isLocalOnly() const               { return localOnly; }
inline bool CollisionDetect::isSendCrashEventsEnabled() const  { return sendCrashEvents; }

}
}

#endif
