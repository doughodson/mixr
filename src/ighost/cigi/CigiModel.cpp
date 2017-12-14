
#include "mixr/ighost/cigi/CigiModel.hpp"

#include "mixr/ighost/cigi/TypeMapper.hpp"

#include "mixr/base/String.hpp"

#include "mixr/simulation/AbstractNib.hpp"

#include "mixr/models/player/Player.hpp"
#include "mixr/models/player/ground/GroundVehicle.hpp"

#include "cigicl/CigiEntityCtrlV3.h"
#include "cigicl/CigiCompCtrlV3.h"
#include "cigicl/CigiArtPartCtrlV3.h"

namespace mixr {
namespace cigi {

IMPLEMENT_SUBCLASS(CigiModel, "CigiModel")
EMPTY_SLOTTABLE(CigiModel)

CigiModel::CigiModel()
{
   STANDARD_CONSTRUCTOR()
}

void CigiModel::copyData(const CigiModel& org, const bool)
{
    BaseClass::copyData(org);

    setPlayer(org.player);

    state = org.state;
    ageCount = org.ageCount;
    checked = org.checked;
    typeMapper = org.typeMapper;
    rcount = org.rcount;
    hotActive = org.hotActive;

    playerID = org.playerID;

    const base::String* pp = org.federateName;
    federateName = pp;
}

void CigiModel::deleteData()
{
    setPlayer(nullptr);

   for (int i = 0; i < CigiHost::NUM_BUFFERS; i++) {
      if (parentEC[i]    != nullptr) { delete parentEC[i];    parentEC[i]    = nullptr; }
      if (trailEC[i]     != nullptr) { delete trailEC[i];     trailEC[i]     = nullptr; }
      if (explosionEC[i] != nullptr) { delete explosionEC[i]; explosionEC[i] = nullptr; }
      if (smokeEC[i]     != nullptr) { delete smokeEC[i];     smokeEC[i]     = nullptr; }
      if (damageCC[i]    != nullptr) { delete damageCC[i];    damageCC[i]    = nullptr; }
      if (animationCC[i] != nullptr) { delete animationCC[i]; animationCC[i] = nullptr; }
      if (attachedEC[i]  != nullptr) { delete attachedEC[i];  attachedEC[i]  = nullptr; }
      if (launcherAPC[i] != nullptr) { delete launcherAPC[i]; launcherAPC[i] = nullptr; }
      if (attachedCC[i]  != nullptr) { delete attachedCC[i];  attachedCC[i]  = nullptr; }
   }
}

// set & ref the player pointer
void CigiModel::setPlayer(models::Player* const p)
{
   if (player != nullptr) {
      player->unref();
      playerID = 0;
      federateName = nullptr;
   }

   player = p;

   if (player != nullptr) {
      player->ref();
      playerID = player->getID();
      const simulation::AbstractNib* nib = player->getNib();
      if (nib != nullptr) {
         federateName = nib->getFederateName();
      } else {
         federateName = nullptr;
      }
   }
}

// initialize the model
void CigiModel::initialize(models::Player* const p, const TypeMapper** const otwModelTable, const int numModels)
{
   isGroundPlayer = p->isClassType(typeid(models::GroundVehicle));

   setPlayer(p);
   state = CigiModel::ACTIVE;
   ageCount = 999;
   hotActive = true;
   rcount = 999;
   checked = true;

   // If the IG model table was provided, then look for a match.
   if (otwModelTable != nullptr && numModels > 0) {
      bool found{};
      for (int i = 0; i < numModels && !found; i++) {
         const TypeMapper* otwTypeMapper{otwModelTable[i]};
         if (otwTypeMapper != nullptr) {
            if (otwTypeMapper->isMatchingPlayerType(p)) {
               // We found a match for our player in the OTW model table!
               typeMapper = otwTypeMapper;
               found = true;
            }
         }
      }
   }
}

// clear the model
void CigiModel::clear()
{
   parentActive = false;
   trailActive = false;
   explosionActive = false;
   smokeActive = false;
   animationActive = false;
   damageActive = false;
   attachedEcActive = false;
   launcherApcActive = false;
   attachedCcActive = false;
   isGroundPlayer = false;

   setPlayer(nullptr);
   state = INACTIVE;
   ageCount = 0;
   checked = false;
   typeMapper = nullptr;
   rcount = 0;
   hotActive = false;
   playerID = 0;
   federateName = nullptr;
}

}
}
