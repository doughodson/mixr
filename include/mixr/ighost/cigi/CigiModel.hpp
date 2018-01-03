
#ifndef __mixr_ighost_cigi_CigiModel_H__
#define __mixr_ighost_cigi_CigiModel_H__

#include "mixr/base/Object.hpp"
#include "mixr/base/safe_ptr.hpp"

#include "mixr/ighost/cigi/CigiHost.hpp"
#include <array>

class CigiEntityCtrlV3;
class CigiCompCtrlV3;
class CigiArtPartCtrlV3;

namespace mixr {
namespace base { class String; }
namespace models { class Player; }
namespace cigi {
class Player2CigiMap;

//------------------------------------------------------------------------------
// Class: CigiModel
// Description: Represents a CIGI system model object or height-of-terrain request.
//              Relates simulation player to IG model relationships.
//
// Notes:
//    1) The 'checked' flag and functions are used by the CigiHost class when
//       rechecking this model with the associated player.
//
//    2) The age counters are used by the IG unique handlers.
//
// Factory name: CigiModel
//------------------------------------------------------------------------------
class CigiModel : public base::Object
{
   DECLARE_SUBCLASS(CigiModel, base::Object)

public:
   enum class State {
      INACTIVE,        // Unused model object
      ACTIVE,          // Player is alive and in-range
      DEAD,            // player is dead or destoryed
      OUT_OF_RANGE,    // Player is alive but out of range
      CLEARED          // Request removel from table
   };

public:
   CigiModel();

   int getID() const                           { return entityId; }
   void setID(const int id)                    { entityId = id; }

   bool isState(const State tst) const         { return (tst == state); } // True if the model is this state ( INACTIVE, ACTIVE, etc. }
   State getState() const                      { return state; }          // Model's state  ( INACTIVE, ACTIVE, etc. }
   void setState(const State newState)         { state = newState; }      // Sets the model's state  ( INACTIVE, ACTIVE, etc. }

   models::Player* getPlayer()                 { return player; }         // The player object associated with this model
   const models::Player* getPlayer() const     { return player; }         // The player object associated with this model (const version)

   const Player2CigiMap* getTypeMapper() const { return typeMapper; }     // IG type mapper

   int getPlayerID() const                     { return playerID; }       // Player ID for the player associated with this model
   const base::String* getFederateName() const { return federateName; }   // Player's federate name (if networked)

   int getAgeCount() const                     { return ageCount; }       // Age counter value (number of IG frames since last IG update)
   int incAgeCount()                           { return ++ageCount; }     // Increments the age counter
   void setAgeCount(const int v)               { ageCount = v; }          // Sets the age counter

   bool isHotActive() const                    { return hotActive; }      // True if a Height-Of-Terrain request is active for this model
   void setHotActive(const bool b)             { hotActive = b; }         // Sets the  Height-Of-Terrain request active flag for this model

   int getReqCount() const                     { return rcount; }         // HOT request age counter value (number of IG frames since last HOT request)
   int incReqCount()                           { return ++rcount; }       // Increments the HOT request age counter
   void setReqCount(const int v)               { rcount = v; }            // Sets the HOT request age counter

   bool isChecked() const                      { return checked; }        // True if this model has been checked with the associated player object
   bool isNotChecked() const                   { return !checked; }       // True if this model has not been checked with the associated player object
   void setCheckedFlag(const bool flg)         { checked = flg; }         // Sets the 'checked' flag

   // Initializes this model for player, 'p' (we're ACTIVE), and
   // looks up the IG model type ID in the model table, 'igModelTable'.
   // If the size of the IG model table is zero(0), then the model type ID is not set.
   void initialize(models::Player* const p, const Player2CigiMap** const igModelTable = nullptr, const int numModels = 0);

   // Clear out this model (we're INACTIVE)
   void clear();

   std::array<CigiEntityCtrlV3*, CigiHost::NUM_BUFFERS> parentEC{};      // (entity_id)   Our main entity
   std::array<CigiEntityCtrlV3*, CigiHost::NUM_BUFFERS> trailEC{};       // (entity_id+1) Trails (missile, smoke, wake, etc.)
   std::array<CigiEntityCtrlV3*, CigiHost::NUM_BUFFERS> explosionEC{};   // (entity_id+2) Expolsions (air or ground)
   std::array<CigiEntityCtrlV3*, CigiHost::NUM_BUFFERS> smokeEC{};       // (entity_id+3) Smoke from damage
   std::array<CigiCompCtrlV3*, CigiHost::NUM_BUFFERS> damageCC{};        // Damage Component Control (CC)
   std::array<CigiCompCtrlV3*, CigiHost::NUM_BUFFERS> animationCC{};     // animation component control
   std::array<CigiEntityCtrlV3*, CigiHost::NUM_BUFFERS> attachedEC{};    // (entity_id+4) Attached missile
   std::array<CigiArtPartCtrlV3*, CigiHost::NUM_BUFFERS> launcherAPC{};  // Entity's launcher Articulated Part Control (APC)
   std::array<CigiCompCtrlV3*, CigiHost::NUM_BUFFERS> attachedCC{};      // attached missile component control

   bool parentActive{};
   bool trailActive{};
   bool explosionActive{};
   bool smokeActive{};
   bool damageActive{};
   bool animationActive{};
   bool attachedEcActive{};
   bool launcherApcActive{};
   bool attachedCcActive{};

   bool isGroundPlayer{};
   double effectsTimer{};

private:
   int entityId{};

   // Sets the player object, p, associated with this model
   void setPlayer(models::Player* const);

   models::Player* player{};             // This player
   State state{State::INACTIVE};         // Model Active flag
   int ageCount{};                       // Age counter (how many times have we've been overlooked)
   bool checked{};                       // Model was checked
   const Player2CigiMap* typeMapper{};   // IG unique model type mapper

   int rcount{};                   // HOT request counter (how many times have we asked)
   bool hotActive{};               // HOT entry is active

   // Model IDs  -- Comparisons in this order --
   int playerID{};                                    // Player ID
   base::safe_ptr<const base::String> federateName;   // Federate name
};

}
}

#endif
