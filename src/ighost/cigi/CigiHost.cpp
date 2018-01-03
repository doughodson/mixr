
#include "mixr/ighost/cigi/CigiHost.hpp"

#include "mixr/ighost/cigi/session/HostSession.hpp"
#include "IgThread.hpp"

#include "mixr/ighost/cigi/CigiModel.hpp"

#include "mixr/ighost/cigi/Player2CigiMap.hpp"

#include "mixr/models/navigation/Navigation.hpp"
#include "mixr/models/player/air/AirVehicle.hpp"
#include "mixr/models/player/effect/Decoy.hpp"
#include "mixr/models/player/effect/Effect.hpp"
#include "mixr/models/player/effect/Flare.hpp"
#include "mixr/models/player/ground/GroundVehicle.hpp"
#include "mixr/models/player/weapon/AbstractWeapon.hpp"
#include "mixr/models/player/ground/SamVehicle.hpp"
#include "mixr/models/player/weapon/Bomb.hpp"
#include "mixr/models/player/weapon/Missile.hpp"
#include "mixr/models/player/Building.hpp"
#include "mixr/models/player/LifeForm.hpp"
#include "mixr/models/player/Player.hpp"
#include "mixr/models/player/Ship.hpp"
#include "mixr/models/player/space/SpaceVehicle.hpp"
#include "mixr/models/system/StoresMgr.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/network/NetHandler.hpp"
#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/SlotTable.hpp"

#include "cigicl/CigiEntityCtrlV3.h"
#include "cigicl/CigiCompCtrlV3.h"
#include "cigicl/CigiIGCtrlV3.h"
#include "cigicl/CigiLosVectReqV3.h"
#include "cigicl/CigiViewCtrlV3.h"
#include "cigicl/CigiViewDefV3.h"
#include "cigicl/CigiArtPartCtrlV3.h"
#include "cigicl/CigiHatHotReqV3.h"
#include "cigicl/CigiSOFV3.h"
#include "cigicl/CigiHatHotRespV3.h"
#include "cigicl/CigiHatHotRespV3_2.h"
#include "cigicl/CigiHatHotXRespV3_2.h"
#include "cigicl/CigiLosRespV3.h"
#include "cigicl/CigiCollDetSegRespV3.h"
#include "cigicl/CigiCollDetVolRespV3.h"
#include "cigicl/CigiSensorRespV3.h"
#include "cigicl/CigiIGMsgV3.h"
#include "cigicl/CigiHostSession.h"
#include "cigicl/CigiBaseSignalProcessing.h"
#include "cigicl/CigiSensorCtrlV3.h"

#include <iostream>

namespace mixr {
namespace cigi {

IMPLEMENT_SUBCLASS(CigiHost, "CigiHost")

BEGIN_SLOTTABLE(CigiHost)
   "session",              // 1) Networked host session (required)
   "async",                // 2) True (non-zero) to run in CIGI async mode (default: false - CIGI sync)
   "hideOwnshipModel",     // 3) True to hide the ownship's model (default: true - ownship's model is not seen)
   "ownshipModel",         // 4) Ownship's model ID
   "mslTrailModel",        // 5) Missile Trail" effect model ID
   "smokePlumeModel",      // 6) Smoke Plume" effect model ID
   "airExplosionModel",    // 7) Air Explosion" effect model ID
   "groundExplosionModel", // 8) Ground Explosion" effect model ID
   "shipWakeModel",        // 9) Ship Wake" effect model ID
END_SLOTTABLE(CigiHost)

BEGIN_SLOT_MAP(CigiHost)
   ON_SLOT(1, setSlotHostSession,             HostSession)
   ON_SLOT(2, setSlotASyncMode,               base::Boolean)
   ON_SLOT(3, setSlotHideOwnshipModel,        base::Number)
   ON_SLOT(4, setSlotOwnshipModelId,          base::Number)
   ON_SLOT(5, setSlotMslTrailModelId,         base::Number)
   ON_SLOT(6, setSlotSmokePlumeModelId,       base::Number)
   ON_SLOT(7, setSlotAirExplosionModelId,     base::Number)
   ON_SLOT(8, setSlotGroundExplosionModelId,  base::Number)
   ON_SLOT(9, setSlotShipWakeModelId,         base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Parameters
//------------------------------------------------------------------------------
static const int MAX_BUF_SIZE{1472};
static const double LOS_REQ_TIMEOUT{2.0};     // one second timeout

CigiHost::CigiHost()
{
   STANDARD_CONSTRUCTOR()

   for (int i = 0; i < NUM_BUFFERS; i++) {
      ownshipEC[i] = new CigiEntityCtrlV3();
      ownshipCC[i] = new CigiCompCtrlV3();
   }

   igc = new CigiIGCtrlV3();
   los = new CigiLosVectReqV3();
   fov = new CigiViewDefV3();
}

void CigiHost::copyData(const CigiHost& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      for (int i = 0; i < NUM_BUFFERS; i++) {
         ownshipEC[i] = new CigiEntityCtrlV3();
         ownshipCC[i] = new CigiCompCtrlV3();
      }
      igc =  new CigiIGCtrlV3();
      los = new CigiLosVectReqV3();
   }

   if (igThread != nullptr) {
      igThread->terminate();
      igThread = nullptr;
   }

   session = nullptr;
   if (org.session != nullptr) {
      session = org.session->clone();
   }
   cigiInitialized = false;
   cigiInitFailed = false;

   asyncMode = org.asyncMode;
   hideOwn = org.hideOwn;
   resetRequest = true;

   entityIdCount = 0;
   elevReqIdCount = 0;

   elevReqFlg = false;
   elevReqTimer = 0;

   losRespLat = 0;
   losRespLon = 0;
   losRespAlt = 0;
   losRespRange = 0;
   losRespId = 0;
   losRespDataValid = true;
   losReqId = 0;
   newLosReq = true;
   losReqTimer = 0;

   iw = NUM_BUFFERS;
   iw0 = NUM_BUFFERS;
   ir = NUM_BUFFERS;

   cmtOwnship = org.cmtOwnship;
   cmtMslTrail = org.cmtMslTrail;
   cmtSmokePlume = org.cmtSmokePlume;
   cmtAirExplosion = org.cmtAirExplosion;
   cmtGroundExplosion = org.cmtGroundExplosion;
   cmtShipWake = org.cmtShipWake;
}

void CigiHost::deleteData()
{
   session = nullptr;

   if (igThread != nullptr) {
      igThread->terminate();
      igThread = nullptr;
   }

   for (int i = 0; i < NUM_BUFFERS; i++) {
      if (ownshipEC[i] != nullptr) { delete ownshipEC[i]; ownshipEC[i] = nullptr; }
      if (ownshipCC[i] != nullptr) { delete ownshipCC[i]; ownshipCC[i] = nullptr; }
   }
   if (igc != nullptr)  { delete igc;  igc = nullptr; }
   if (los != nullptr)  { delete los;  los = nullptr; }
   if (view != nullptr) { delete view; view = nullptr; }
   if (fov != nullptr)  { delete fov;  fov = nullptr; }
   iw = NUM_BUFFERS;
   iw0 = NUM_BUFFERS;
   ir = NUM_BUFFERS;
}

// creates a thread to receive and process incoming IG packets
bool CigiHost::createProcessingThread()
{
   if ( igThread == nullptr ) {

      // parent -> our IG manager
      igThread = new IgThread(this);
      igThread->unref(); // 'singleTask' is a safe_ptr<>

      bool ok{igThread->start(0.6)};
      if (!ok) {
         igThread = nullptr;
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "CigiHost::createProcessingThread(): ERROR, failed to create thread!" << std::endl;
         }
      }
   }
   return (igThread != nullptr);
}

// create Cigi model objects
CigiModel* CigiHost::modelFactory()
{
   const auto p = new CigiModel();
   p->setID( ++entityIdCount );
   return p;
}

// create Cigi Hot objects
CigiModel* CigiHost::hotFactory()
{
   const auto p = new CigiModel();
   p->setID( ++elevReqIdCount );
   return p;
}

void CigiHost::updateData(const double dt)
{
   // ---
   // init cigi session handler
   // ---
   if (session != nullptr && !cigiInitialized && !cigiInitFailed) {
      cigiInitialized = session->initialize(this);
      cigiInitFailed = !cigiInitialized;
   }

   // initialize thread
   if (igThread == nullptr) {
      createProcessingThread();
   }

   // ---
   // LOS request timeout control
   // ---
   if ( isLosRequestPending() ) {
      losReqTimer -= dt;
      if (losReqTimer <= 0.0) {
         // timeout!!!!
         losRespDataValid = false;
         losRespId = losReqId;
         losReqTimer = 0.0;
      }
   }

   // ---
   // Elevation request timeout control
   // ---
   if ( elevReqFlg ) {
      elevReqTimer -= dt;
      if (elevReqTimer <= 0.0) {
         // timeout!!!!
         elevReqFlg = false;
         elevReqTimer = 0.0;
      }
   }

   // Update base classes stuff
   BaseClass::updateData(dt);
}

// send state data for ownship and models
void CigiHost::sendOwnshipAndModels()
{
   // update the write buffer index
   iw++;
   if (iw >= NUM_BUFFERS) iw = 0;

   // Update ownship state entity
   bool ok{updateOwnshipModel()};

   if (ok) {
      // only if we have a good ownship model update our models
      updateModels();
   }

   // set the index of the last write buffer
   iw0 = iw;
}

// sends terrain height requests
void CigiHost::sendElevationRequests()
{
   // Handled by Start-Of-Frame
}

// receives terrain height data
void CigiHost::recvElevations()
{
   // Handled by CIGI callback
}

// trigger the frame update
void CigiHost::frameSync()
{
   if (session != nullptr && session->isInitialized()) {
      if (isASyncMode()) {
         sendCigiData();
      }
   }
}

// reset host interface
void CigiHost::reset()
{
   BaseClass::reset();

   losRespDataValid = false;
   newLosReq = false;
   losReqTimer = false;
   losRespId = losReqId;

   resetRequest = true;
}

// update the ownship model; returns true if ok
bool CigiHost::updateOwnshipModel()
{
   // Ownship active and type air vehicle?
//   bool active = false;
//   if (getOwnship() != nullptr) {
//      active = getOwnship()->isActive() || getOwnship()->isMode(simulation::Player::PRE_RELEASE);
//   }

//   const simulation::Player* av = getOwnship();

   // code above changed to this by DDH -- NOTE, this appears to be wrong, not AirVehicle!
   bool active{};
   const auto av = dynamic_cast<const models::Player*>(getOwnship());
   if (av != nullptr) {
      active = av->isActive() || av->isMode(models::Player::PRE_RELEASE);
   }

   if (active && av != nullptr && getOwnshipEntityControlPacket(iw) != nullptr) {
      // We have an active, AirVehicle type ownship and an entity control packet ...

      CigiEntityCtrlV3* ec{getOwnshipEntityControlPacket(iw)};
      setCommonModelData(ec, 0, av); // Note: Ownship is always entity #0
      if (isOwnshipModelHidden()) {
         ec->SetEntityState(CigiEntityCtrlV3::Standby);
      } else {
         // if we are being shown, make sure we update our model
         ec->SetEntityState(CigiEntityCtrlV3::Active);
      }
      ec->SetEntityType(static_cast<Cigi_uint16>(cmtOwnship));
      ec->SetAttachState(CigiEntityCtrlV3::Detach);
      ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
      ec->SetAnimationState(CigiEntityCtrlV3::Stop);
      ec->SetAlpha(255);
      if (getOwnshipComponentControlPacket(iw) != nullptr) {
         const auto player = dynamic_cast<const models::LifeForm*>(getOwnship());
         CigiCompCtrlV3* animation{ownshipCC[iw]};
         if (animation != nullptr) {
            animation->SetCompClassV3(CigiCompCtrlV3::EntityV3);
            animation->SetInstanceID(0);
            animation->SetCompID(8); // Change Model Animation
            animation->SetCompState(LIFE_FORM_STATE::STANDING);
            animation->SetCompData(0.0f, 0);
            animation->SetCompData(0.0f, 1);

            if (player != nullptr) {
               // get our ownship models id and our model
               const auto lf = const_cast<models::LifeForm*>(static_cast<const models::LifeForm*>(player));
               if (lf != nullptr) {
                  if (lf->getDamage() < 1) {
                     // Choose Animation state
                     if (lf->getActionState() == models::LifeForm::UPRIGHT_STANDING) {
                        animation->SetCompState(STANDING);
                     } else if (lf->getActionState() == models::LifeForm::UPRIGHT_WALKING) {
                        animation->SetCompState(WALK);
                     } else if (lf->getActionState() == models::LifeForm::UPRIGHT_RUNNING) {
                        animation->SetCompState(RUN);
                     } else {
                        // default is standing
                        animation->SetCompState(STANDING);
                     }
                  } else {
                     animation->SetCompState(DEAD);
                  }
               }
            }
         }
      }
   }

   return true;
}

// update the other models; returns number of active models
int CigiHost::updateModels()
{
   int n{};

   // Do we have models?
   CigiModel** const table{getModelTable()};
   if (table != nullptr && getModelTableSize() > 0) {

      // For all active models in the table ...
      for (int i = 0; i < getModelTableSize(); i++) {
         base::safe_ptr<CigiModel> model( static_cast<CigiModel*>(table[i]) );
         if (model != nullptr) {

            if (model->getState() != CigiModel::State::INACTIVE) {
               int entity{model->getID()* 8 + 1};      // Save a block of four entities per model
               //  (id*8+1) is parent entity
               //  (id*8+2) is smoke trail entity
               //  (id*8+3) is air explosion entity
               //  (id*8+4) is ground explosion entity
               //  (id*8+5) is attached part entity

               // Get the player
               const auto player = dynamic_cast<const models::Player*>(model->getPlayer());  // DDH

               // Set the model data and ...
               if (player->isMajorType(models::Player::AIR_VEHICLE)) {
                  setAirVehicleData(model, entity, static_cast<const models::AirVehicle*>(player));
               } else if (player->isMajorType(models::Player::GROUND_VEHICLE)) {
                  setGndVehicleData(model, entity, static_cast<const models::GroundVehicle*>(player));
               } else if (player->isMajorType(models::Player::SHIP)) {
                  setShipData(model, entity, static_cast<const models::Ship*>(player));
               } else if (player->isMajorType(models::Player::SPACE_VEHICLE)) {
                  setSpaceVehicleData(model, entity, static_cast<const models::SpaceVehicle*>(player));
               } else if (player->isMajorType(models::Player::LIFE_FORM)) {
                  setLifeFormData(model, entity, static_cast<const models::LifeForm*>(player));
               } else if (player->isMajorType(models::Player::BUILDING)) {
                  setBuildingData(model, entity, static_cast<const models::Building*>(player));
               } else if (player->isMajorType(models::Player::WEAPON)) {
                  const auto effect = dynamic_cast<const models::Effect*>(model->getPlayer());
                  const auto msl = dynamic_cast<const models::Missile*>(model->getPlayer());
                  const auto wpn = dynamic_cast<const models::AbstractWeapon*>(model->getPlayer());
                  if (effect != nullptr)     // Effects before general weapons (because effects are also weapons)
                     setEffectData(model, entity, effect);
                  else if (msl != nullptr)   // Missiles before general weapons (because missiles are also weapons)
                     setMissileData(model, entity, msl);
                  else if (wpn != nullptr)
                     setWeaponData(model, entity, wpn);
               }

            }
         }
      }
   }

   return n;
}

// sets a CigiEntityCtrlV3 structure with common data entity data
bool CigiHost::setCommonModelData(CigiEntityCtrlV3* const ec, const int entity, const models::Player* const p)
{
   bool ok{ec != nullptr && p != nullptr};

   if (ok) {
      // Set entity id, parent and state
      ec->SetEntityID(entity);
      ec->SetParentID(0);
      //ec->parent_id = -1;

      // Set angles
      ec->SetRoll(static_cast<float>(p->getRollD()));
      ec->SetPitch(static_cast<float>(p->getPitchD()));
      float hdg{static_cast<float>(p->getHeadingD())};
      if (hdg < 0.0) hdg += 360.0f;
      if (hdg >= 360.0f) hdg = 0.0;
      ec->SetYaw(hdg, false);

      // Set position
      ec->SetLat(p->getLatitude());
      ec->SetLon(p->getLongitude());
      ec->SetAlt(p->getAltitude());
   }
   return ok;
}

// sets a CigiEntityCtrlV3 structure to an air vheicle's state
bool CigiHost::setAirVehicleData(CigiModel* const m, const int entity, const models::AirVehicle* const p)
{
   // Make sure we have an entity control block
   if (m->parentEC[iw] == nullptr) {
      m->parentEC[iw] = new CigiEntityCtrlV3();
   }
   CigiEntityCtrlV3* const ec{m->parentEC[iw]};

   // Smoke control block, ...
   if (m->smokeEC[iw] == nullptr) {
      m->smokeEC[iw] = new CigiEntityCtrlV3();
      m->smokeActive = false;
   }
   CigiEntityCtrlV3* const smoke{m->smokeEC[iw]};

   // As long as we're active, update the entity control
   if (m->getState() == CigiModel::State::ACTIVE) {
      bool ok{setCommonModelData(ec, entity, p)};
      if (ok) {
         // Set the entity type
         int tt{};
         const Player2CigiMap* otm {m->getTypeMapper()};
         if (otm != nullptr) tt = otm->getEntityId();
         if (tt > 0xffff) tt = 0;
         ec->SetEntityType(static_cast<Cigi_uint16>(tt));

         // Set entity control data
         ec->SetEntityState(CigiEntityCtrlV3::Active);
         ec->SetAttachState(CigiEntityCtrlV3::Detach);
         ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         ec->SetAnimationState(CigiEntityCtrlV3::Stop);
         ec->SetAlpha(255);
      }
      m->parentActive = ok;

      // Load the smoke entity control
      if (p->getSmoke() > 0.3) {
         // Set entity id, parent and state
         smoke->SetEntityID(entity+3); // Smoke is at main entity id plus one
         smoke->SetParentID(entity);

         smoke->SetRoll(0.0);
         smoke->SetPitch(0.0);
         smoke->SetYaw(0.0);
         smoke->SetAlt(0.0);
         smoke->SetLat(0.0);
         smoke->SetLon(0.0);

         smoke->SetEntityState(CigiEntityCtrlV3::Active);
         smoke->SetEntityType(static_cast<Cigi_uint16>(cmtSmokePlume));
         smoke->SetAttachState(CigiEntityCtrlV3::Attach);
         smoke->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         //smoke->SetAnimationState(CigiEntityCtrlV3::Stop);
         smoke->SetAnimationState(CigiEntityCtrlV3::Continue);
         smoke->SetAlpha(170);
         m->smokeActive = true;
      } else {
         smoke->SetEntityState(CigiEntityCtrlV3::Standby);
      }
   } else {
      ec->SetEntityState(CigiEntityCtrlV3::Standby);
      smoke->SetEntityState(CigiEntityCtrlV3::Standby);
   }
   return true;
}

// sets a 'model_t' structure to a building's state
bool CigiHost::setBuildingData(CigiModel* const m, const int entity, const models::Building* const p)
{
   // Make sure we have an entity control block
   if (m->parentEC[iw] == nullptr) {
      m->parentEC[iw] = new CigiEntityCtrlV3();
   }
   CigiEntityCtrlV3* const ec{m->parentEC[iw]};

   // Smoke control block, ...
   if (m->smokeEC[iw] == nullptr) {
      m->smokeEC[iw] = new CigiEntityCtrlV3();
      m->smokeActive = false;
   }
   CigiEntityCtrlV3* const smoke{m->smokeEC[iw]};

   // Damage control, ...
   if (m->damageCC[iw] == nullptr) {
      m->damageCC[iw] = new CigiCompCtrlV3();
      m->damageCC[iw]->SetCompClassV3(CigiCompCtrlV3::EntityV3);
      m->damageCC[iw]->SetInstanceID(entity);
      m->damageCC[iw]->SetCompID(6);            // Object State
      m->damageCC[iw]->SetCompState(1);         // 0 = DEFAULT, 1 = GOOD, 2 = DAMAGED, 3 = DISTROYED
      m->damageCC[iw]->SetCompData(0.0f, 0);
      m->damageCC[iw]->SetCompData(0.0f, 1);
      m->damageActive = true;                   // true the first time to make sure the model state is good
   }
   CigiCompCtrlV3* const damage{m->damageCC[iw]};

   // As long as we're active, update the entity control
   if (m->getState() == CigiModel::State::ACTIVE) {
      bool ok{setCommonModelData(ec, entity, p)};
      if (ok) {
         // Set the entity type
         int tt{};
         const Player2CigiMap* otm {m->getTypeMapper()};
         if (otm != nullptr) tt = otm->getEntityId();
         if (tt > 0xffff) tt = 0;
         ec->SetEntityType(static_cast<Cigi_uint16>(tt));

         // Set entity control data
         ec->SetEntityState(CigiEntityCtrlV3::Active);
         ec->SetAttachState(CigiEntityCtrlV3::Detach);
         ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         ec->SetAnimationState(CigiEntityCtrlV3::Stop);
         ec->SetAlpha(255);
      }
      m->parentActive = ok;

      // Load the smoke entity control

      if (p->getSmoke() > 0.1f) {
         // Set entity id, parent and state
         smoke->SetEntityID(entity+3); // Smoke is at main entity id plus one
         smoke->SetParentID(entity);

         smoke->SetRoll(0.0);
         smoke->SetPitch(0.0);
         smoke->SetYaw(0.0);
         smoke->SetAlt(0.0);
         smoke->SetLat(0.0);
         smoke->SetLon(0.0);

         smoke->SetEntityState(CigiEntityCtrlV3::Active);
         smoke->SetEntityType(static_cast<Cigi_uint16>(cmtSmokePlume));
         smoke->SetAttachState(CigiEntityCtrlV3::Attach);
         smoke->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         //smoke->SetAnimationState(CigiEntityCtrlV3::Stop);
         smoke->SetAnimationState(CigiEntityCtrlV3::Continue);
         smoke->SetAlpha(170);
         m->smokeActive = true;
      } else {
         smoke->SetEntityState(CigiEntityCtrlV3::Standby);
      }

      if (p->getDamage() > 0.0) {
         if (p->isDestroyed()) {
            damage->SetCompState(3);    // 3 = DISTROYED
         } else {
            damage->SetCompState(2);    // 2 = DAMAGED
         }
         m->damageActive = true;
      } else {
         damage->SetCompState(1);       // 0 = DEFAULT, 1 = GOOD, 2 = DAMAGED, 3 = DISTROYED
      }
   } else {
      ec->SetEntityState(CigiEntityCtrlV3::Standby);
      smoke->SetEntityState(CigiEntityCtrlV3::Standby);
   }
   return true;
}

// sets a 'model_t' structure to a ground vheicle's state
bool CigiHost::setGndVehicleData(CigiModel* const m, const int entity, const models::GroundVehicle* const p)
{
   // Make sure we have an entity control block
   if (m->parentEC[iw] == nullptr) {
      m->parentEC[iw] = new CigiEntityCtrlV3();
   }
   CigiEntityCtrlV3* const ec{m->parentEC[iw]};

   // Smoke control block, ...
   if (m->smokeEC[iw] == nullptr) {
      m->smokeEC[iw] = new CigiEntityCtrlV3();
      m->smokeActive = false;
   }
   CigiEntityCtrlV3* const smoke{m->smokeEC[iw]};

   // Damage control, ...
   if (m->damageCC[iw] == nullptr) {
      m->damageCC[iw] = new CigiCompCtrlV3();
      m->damageCC[iw]->SetCompClassV3(CigiCompCtrlV3::EntityV3);
      m->damageCC[iw]->SetInstanceID(entity);
      m->damageCC[iw]->SetCompID(6);       // Object State
      m->damageCC[iw]->SetCompState(1);    // 0 = DEFAULT, 1 = GOOD, 2 = DAMAGED, 3 = DISTROYED
      m->damageCC[iw]->SetCompData(0.0f, 0);
      m->damageCC[iw]->SetCompData(0.0f, 1);
      m->damageActive = true;  // true the first time to make sure the model state is good
   }
   CigiCompCtrlV3* const damage {m->damageCC[iw]};

   // Articulated launcher control
   if (m->launcherAPC[iw] == nullptr) {
      m->launcherAPC[iw] = new CigiArtPartCtrlV3();
      m->launcherApcActive = false;
   }
   CigiArtPartCtrlV3* const launcherAPC{m->launcherAPC[iw]};

   // Attached part, ...
   if (m->attachedEC[iw] == nullptr) {
      m->attachedEC[iw] = new CigiEntityCtrlV3();
      m->attachedEcActive = false;
   }
   CigiEntityCtrlV3* const attachedPartEC{m->attachedEC[iw]};

   // Damage control, ...
   if (m->attachedCC[iw] == nullptr) {
      m->attachedCC[iw] = new CigiCompCtrlV3();
      m->attachedCcActive = false;
   }
   CigiCompCtrlV3* const attachedPartCC{m->attachedCC[iw]};

   // As long as we're active, update the entity control
   if (m->getState() == CigiModel::State::ACTIVE) {
      bool ok{setCommonModelData(ec, entity, p)};
      if (ok) {

         // Set the entity type
         int tt{};
         const Player2CigiMap* otm{m->getTypeMapper()};
         if (otm != nullptr) tt = otm->getEntityId();
         if (tt > 0xffff) tt = 0;
         ec->SetEntityType(static_cast<Cigi_uint16>(tt));

         // Set entity control data
         ec->SetEntityState(CigiEntityCtrlV3::Active);
         ec->SetAttachState(CigiEntityCtrlV3::Detach);
         ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         ec->SetAnimationState(CigiEntityCtrlV3::Stop);
         ec->SetAlpha(255);
      }
      m->parentActive = ok;

      // Load the smoke entity control
      if (p->getSmoke() > 0.1f) {
         // Set entity id, parent and state
         smoke->SetEntityID(entity+3); // Smoke is at main entity id plus one
         smoke->SetParentID(entity);

         smoke->SetRoll(0.0);
         smoke->SetPitch(0.0);
         smoke->SetYaw(0.0);
         smoke->SetAlt(0.0);
         smoke->SetLat(0.0);
         smoke->SetLon(0.0);

         smoke->SetEntityState(CigiEntityCtrlV3::Active);
         smoke->SetEntityType(static_cast<Cigi_uint16>(cmtSmokePlume));
         smoke->SetAttachState(CigiEntityCtrlV3::Attach);
         smoke->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         //smoke->SetAnimationState(CigiEntityCtrlV3::Stop);
         smoke->SetAnimationState(CigiEntityCtrlV3::Continue);
         smoke->SetAlpha(170);
         m->smokeActive = true;
      } else {
         smoke->SetEntityState(CigiEntityCtrlV3::Standby);
      }

      if (p->getDamage() > 0.0) {
         if (p->isDestroyed()) {
            damage->SetCompState(3);   // 3 = DISTROYED
         } else {
            damage->SetCompState(2);   // 2 = DAMAGED
         }
         m->damageActive = true;
      } else {
         damage->SetCompState(1);      // 0 = DEFAULT, 1 = GOOD, 2 = DAMAGED, 3 = DISTROYED
      }

   } else {
      ec->SetEntityState(CigiEntityCtrlV3::Standby);
      smoke->SetEntityState(CigiEntityCtrlV3::Standby);
   }

   // NAZ-543 Articulated launcher and attached missile
   if (ec->GetEntityType() == 971) {

      int apartNumMissiles{};     // Number of attached missiles

      // find all attached missiles
      const models::StoresMgr* sm{p->getStoresManagement()};
      if (sm != nullptr) {
         const base::PairStream* stores{sm->getStores()};
         if (stores != nullptr) {
            const base::List::Item* item{stores->getFirstItem()};
            while (item != nullptr && apartNumMissiles == 0) {
               const auto pair = static_cast<const base::Pair*>(item->getValue());
               if (pair != nullptr) {
                  const auto msl = dynamic_cast<const models::Missile*>( pair->object() );
                  if (msl != nullptr) apartNumMissiles++;
               }
               item = item->getNext();
            }
            stores->unref();
            stores = nullptr;
         }
      }

      // Launcher articulation
      launcherAPC->SetEntityID(entity);
      launcherAPC->SetArtPartID(1);       // for MAZ-543; 1 is the launcher
      launcherAPC->SetPitchEn(true);  // Pitch enabled
      launcherAPC->SetPitch( static_cast<float>(p->getLauncherPosition() * base::angle::R2DCC) );
      m->launcherApcActive = true;

      // Attached missile
      attachedPartEC->SetEntityID(entity + 4);  // attached part
      attachedPartEC->SetParentID(entity);
      attachedPartEC->SetRoll(0.0f);
      attachedPartEC->SetPitch(0.0f);
      attachedPartEC->SetYaw(0.0f);
      attachedPartEC->SetAlt(0.0f);
      attachedPartEC->SetLat(0.0);
      attachedPartEC->SetLon(0.0);
      attachedPartEC->SetEntityType(917);     // SCUD missile type
      attachedPartEC->SetAttachState(CigiEntityCtrlV3::Attach);
      attachedPartEC->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
      attachedPartEC->SetAnimationState(CigiEntityCtrlV3::Stop);
      attachedPartEC->SetAlpha(255);
      if (apartNumMissiles > 0) {

         attachedPartEC->SetEntityState(CigiEntityCtrlV3::Active);
         m->attachedEcActive = true;

         // Attached missile to launcher's articulation
         attachedPartCC->SetCompClassV3(CigiCompCtrlV3::EntityV3);
         attachedPartCC->SetInstanceID(attachedPartEC->GetEntityID());
         attachedPartCC->SetCompID(9); // 9 value is for stores attachment to locations
         attachedPartCC->SetCompState(0);
         attachedPartCC->SetCompData(0.0f, 0);
         attachedPartCC->SetCompData(0.0f, 1);
         m->attachedCcActive = true;

      } else {
         attachedPartEC->SetEntityState(CigiEntityCtrlV3::Standby);
      }

   } else {
      // Not articulation or attached parts
      launcherAPC->SetPitchEn(false);
      attachedPartEC->SetEntityState(CigiEntityCtrlV3::Standby);
   }
   return true;
}

//------------------------------------------------------------------------------
// setEffectsData() -- Sets a 'model_t' structure to a effects' state
//------------------------------------------------------------------------------
bool CigiHost::setEffectData(CigiModel* const m, const int entity, const models::Effect* const p)
{
   // Make sure we have an entity control block
   if (m->parentEC[iw] == nullptr) {
      m->parentEC[iw] = new CigiEntityCtrlV3();
   }
   CigiEntityCtrlV3* const ec{m->parentEC[iw]};

   // a smoke trail entity control block, ...
   if (m->trailEC[iw] == nullptr) {
      m->trailEC[iw] = new CigiEntityCtrlV3();
      m->trailActive = false;
   }
   CigiEntityCtrlV3* const trail{m->trailEC[iw]};

   if (m->getState() == CigiModel::State::ACTIVE) {

      // As long as we're active, update the entity control
      if (p->isClassType(typeid(models::Decoy))) {
         // Decoy

         // Load the parent entity control
         setCommonModelData(ec, entity, p);

         // Set the entity type
         int tt{};
         const Player2CigiMap* otm {m->getTypeMapper()};
         if (otm != nullptr) tt = otm->getEntityId();
         if (tt > 0xffff) tt = 0;
         ec->SetEntityType(static_cast<Cigi_uint16>(tt));

         // Set entity control data
         ec->SetEntityState(CigiEntityCtrlV3::Active);
         ec->SetAttachState(CigiEntityCtrlV3::Detach);
         ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         ec->SetAnimationState(CigiEntityCtrlV3::Stop);
         ec->SetAlpha(255);
         m->parentActive = true;

         // Load the smoke trail entity control
         {
            // Set entity id, parent and state
            trail->SetEntityID(entity+1); // Trails are at main entity id plus one
            trail->SetParentID(entity);

            trail->SetRoll(0.0f);
            trail->SetPitch(0.0f);
            trail->SetYaw(0.0f);
            trail->SetAlt(0.0f);
            trail->SetLat(0.0);
            trail->SetLon(0.0);

            trail->SetEntityState(CigiEntityCtrlV3::Active);
            trail->SetEntityType(static_cast<Cigi_uint16>(cmtMslTrail));
            trail->SetAttachState(CigiEntityCtrlV3::Attach);
            trail->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
            trail->SetAnimationState(CigiEntityCtrlV3::Stop);
            //trail->SetAnimationState(CigiEntityCtrlV3::Continue);
            trail->SetAlpha(170);
            m->trailActive = true;
         }
      }

      else if (p->isClassType(typeid(models::Flare))) {
         // FLARES
         bool ok{setCommonModelData(ec,entity,p)};
         if (ok) {

            // Set the entity type
            int tt{};
            const Player2CigiMap* otm {m->getTypeMapper()};
            if (otm != nullptr) tt = otm->getEntityId();
            if (tt > 0xffff) tt = 0;
            ec->SetEntityType(static_cast<Cigi_uint16>(tt));

            // Set entity control data
            ec->SetEntityState(CigiEntityCtrlV3::Active);
            ec->SetAttachState(CigiEntityCtrlV3::Detach);
            ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
            ec->SetAnimationState(CigiEntityCtrlV3::Continue);
            ec->SetAlpha(255);
         }
         m->parentActive = ok;
      } else {
         // Unknown type
         ec->SetEntityState(CigiEntityCtrlV3::Standby);
         trail->SetEntityState(CigiEntityCtrlV3::Standby);
      }
   } else {
      ec->SetEntityState(CigiEntityCtrlV3::Standby);
      trail->SetEntityState(CigiEntityCtrlV3::Standby);
   }

   return true;
}

//------------------------------------------------------------------------------
// setLifeFormData() -- Sets a 'model_t' structure to a lifeform's state
//------------------------------------------------------------------------------
bool CigiHost::setLifeFormData(CigiModel* const m, const int entity, const models::LifeForm* const p)
{
   // Make sure we have an entity control block
   if (m->parentEC[iw] == nullptr) {
      m->parentEC[iw] = new CigiEntityCtrlV3();
   }
   CigiEntityCtrlV3* const ec{m->parentEC[iw]};

   // Animation control, ...
   if (m->animationCC[iw] == nullptr) {
      m->animationCC[iw] = new CigiCompCtrlV3();
      m->animationCC[iw]->SetCompClassV3(CigiCompCtrlV3::EntityV3);
      m->animationCC[iw]->SetInstanceID(entity);
      m->animationCC[iw]->SetCompID(8); // Change Model Animation
      m->animationCC[iw]->SetCompState(STANDING);
      m->animationCC[iw]->SetCompData(0.0f, 0);
      m->animationCC[iw]->SetCompData(0.0f, 1);
      m->animationActive = true;  // true the first time to make sure the model state is good
   }
   CigiCompCtrlV3* const animation{m->animationCC[iw]};

   // As long as we're active, update the entity control
   if (m->getState() == CigiModel::State::ACTIVE) {
      bool ok{setCommonModelData(ec, entity, p)};
      if (ok) {
         // Set the entity type
         int tt{};
         const Player2CigiMap* otm{m->getTypeMapper()};
         if (otm != nullptr) tt = otm->getEntityId();
         if (tt > 0xffff) tt = 0;
         ec->SetEntityType(static_cast<Cigi_uint16>(tt));

         // Set entity control data
         ec->SetEntityState(CigiEntityCtrlV3::Active);
         ec->SetAttachState(CigiEntityCtrlV3::Detach);
         ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         ec->SetAnimationState(CigiEntityCtrlV3::Stop);
         ec->SetAlpha(255);
      }
      m->parentActive = ok;

      const auto lf = const_cast<models::LifeForm*>(static_cast<const models::LifeForm*>(p));

      if (lf->getDamage() <= 0.9f) {
         // Choose Animation state
         if (lf->getActionState() == models::LifeForm::UPRIGHT_STANDING) {
            animation->SetCompState(STANDING);
         } else if (lf->getActionState() == models::LifeForm::UPRIGHT_WALKING) {
            animation->SetCompState(WALK);
         } else if (lf->getActionState() == models::LifeForm::UPRIGHT_RUNNING) {
            animation->SetCompState(RUN);
         } else {
            // default is standing
            animation->SetCompState(STANDING);
         }
      } else {
         // our damage is enough to kill us, so we are DEAD
         animation->SetCompState(DEAD);
      }
      m->animationActive = true;
   } else {
      ec->SetEntityState(CigiEntityCtrlV3::Standby);
   }
   return true;
}

//------------------------------------------------------------------------------
// setMissileData() -- Sets a 'model_t' structure to a missile's state
//------------------------------------------------------------------------------
bool CigiHost::setMissileData(CigiModel* const m, const int entity, const models::Missile* const p)
{
   // Make sure we have an entity control block, ...
   if (m->parentEC[iw] == nullptr) {
      m->parentEC[iw] = new CigiEntityCtrlV3();
      m->parentActive = false;
   }
   CigiEntityCtrlV3* const ec{m->parentEC[iw]};

   // a smoke trail entity control block, ...
   if (m->trailEC[iw] == nullptr) {
      m->trailEC[iw] = new CigiEntityCtrlV3();
      m->trailActive = false;
   }
   CigiEntityCtrlV3* const trail{m->trailEC[iw]};

   // an air explosion entity control block, ...
   if (m->explosionEC[iw] == nullptr) {
      m->explosionEC[iw] = new CigiEntityCtrlV3();
      m->explosionEC[iw]->SetEntityID(entity+2); // Explosions are at main entity_id plus two
      m->explosionEC[iw]->SetParentID(entity);

      m->explosionEC[iw]->SetRoll(0.0f);
      m->explosionEC[iw]->SetPitch(0.0f);
      m->explosionEC[iw]->SetYaw(0.0f);
      m->explosionEC[iw]->SetAlt(0.0f);
      m->explosionEC[iw]->SetLat(0.0);
      m->explosionEC[iw]->SetLon(0.0);

      m->explosionEC[iw]->SetEntityState(CigiEntityCtrlV3::Active);
      m->explosionEC[iw]->SetEntityType(static_cast<Cigi_uint16>(cmtAirExplosion));
      m->explosionEC[iw]->SetAttachState(CigiEntityCtrlV3::Detach);
      m->explosionEC[iw]->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
      m->explosionEC[iw]->SetAnimationState(CigiEntityCtrlV3::Stop);
      m->explosionEC[iw]->SetAlpha(255);
      m->explosionActive = true;  // True the first time to make sure it's loaded
      //if (isMessageEnabled(MSG_DEBUG)) {
      //std::cout << "explosion: create" << m->explosionEC[iw]->entity_id << std::endl;
      //}
   }
   CigiEntityCtrlV3* const explosion{m->explosionEC[iw]};

   // As long as we're active, update the entity control
   if (m->getState() == CigiModel::State::ACTIVE) {
      // Load the parent entity control
      setCommonModelData(ec, entity, p);

      // Set the entity type
      int tt{};
      const Player2CigiMap* otm{m->getTypeMapper()};
      if (otm != nullptr) tt = otm->getEntityId();
      if (tt > 0xffff) tt = 0;
      ec->SetEntityType(static_cast<Cigi_uint16>(tt));

      // Set entity control data
      ec->SetEntityState(CigiEntityCtrlV3::Active);
      ec->SetAttachState(CigiEntityCtrlV3::Detach);
      ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
      ec->SetAnimationState(CigiEntityCtrlV3::Stop);
      ec->SetAlpha(255);
      m->parentActive = true;

      // Load the smoke trail entity control
      {
         // Set entity id, parent and state
         trail->SetEntityID(entity+1); // Trails are at main entity id plus one
         trail->SetParentID(entity);

         trail->SetRoll(0.0f);
         trail->SetPitch(0.0f);
         trail->SetYaw(0.0f);
         trail->SetAlt(0.0f);
         trail->SetLat(0.0);
         trail->SetLon(0.0);

         //trail->Seteffect_state = EFFECT_ANIMATION_STATE_PLAY;
         trail->SetEntityState(CigiEntityCtrlV3::Active);
         trail->SetEntityType(static_cast<Cigi_uint16>(cmtMslTrail));
         trail->SetAttachState(CigiEntityCtrlV3::Attach);
         trail->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         trail->SetAnimationState(CigiEntityCtrlV3::Stop);
         //trail->SetAnimationState(CigiEntityCtrlV3::Continue);
         trail->SetAlpha(170);
         m->trailActive = true;
      }
   } else {
      ec->SetEntityState(CigiEntityCtrlV3::Standby);
      trail->SetEntityState(CigiEntityCtrlV3::Standby);
      if (m->getState() == CigiModel::State::DEAD) {
         // Start air explosion at last known location of missile
         explosion->SetRoll(ec->GetRoll());
         explosion->SetPitch(ec->GetPitch());
         float yaw{ec->GetYaw()};
         if (yaw < 0.0) yaw += 360.0f;
         if (yaw >= 360.0f) yaw = 0.0;
         explosion->SetYaw(yaw,false);
         if (ec->GetAlt() > p->getTerrainElevation())
            explosion->SetAlt(ec->GetAlt());
         else
            explosion->SetAlt(p->getTerrainElevation());
         explosion->SetLat(ec->GetLat());
         explosion->SetLon(ec->GetLon());
         //explosion->SetAnimationState(CigiEntityCtrlV3::Continue);
         explosion->SetAnimationState(CigiEntityCtrlV3::Play);
         m->explosionActive = true;  // True to start to sequence
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// setShipData() -- Sets a 'model_t' structure to a ship's state
//------------------------------------------------------------------------------
bool CigiHost::setShipData(CigiModel* const m, const int entity, const models::Ship* const p)
{
   // Make sure we have an entity control block
   if (m->parentEC[iw] == nullptr) {
      m->parentEC[iw] = new CigiEntityCtrlV3();
   }
   CigiEntityCtrlV3* const ec{m->parentEC[iw]};

   // a wake entity control block, ...
   if (m->trailEC[iw] == nullptr) {
      m->trailEC[iw] = new CigiEntityCtrlV3();
      m->trailActive = false;
   }
   CigiEntityCtrlV3* const wake{m->trailEC[iw]};

   // As long as we're active, update the entity control
   if (m->getState() == CigiModel::State::ACTIVE) {
      bool ok{setCommonModelData(ec, entity, p)};
      if (ok) {

         // Set the entity type
         int tt{};
         const Player2CigiMap* otm {m->getTypeMapper()};
         if (otm != nullptr) tt = otm->getEntityId();
         if (tt > 0xffff) tt = 0;
         ec->SetEntityType(static_cast<Cigi_uint16>(tt));

         // Set entity control data
         ec->SetEntityState(CigiEntityCtrlV3::Active);
         ec->SetAttachState(CigiEntityCtrlV3::Detach);
         ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         ec->SetAnimationState(CigiEntityCtrlV3::Stop);
         ec->SetAlpha(255);
      }
      m->parentActive = ok;

      // Load the wake entity control
      {
         // Set entity id, parent and state
         wake->SetEntityID(entity+1); // Trails (wakes) are at parent entity plus one
         wake->SetParentID(entity);

         wake->SetRoll(0.0f);
         wake->SetPitch(0.0f);
         wake->SetYaw(0.0f);
         wake->SetAlt(0.0f);
         wake->SetLat(0.0);
         wake->SetLon(0.0);

         wake->SetEntityState(CigiEntityCtrlV3::Active);
         wake->SetEntityType(static_cast<Cigi_uint16>(cmtShipWake));
         wake->SetAttachState(CigiEntityCtrlV3::Attach);
         wake->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         wake->SetAnimationState(CigiEntityCtrlV3::Continue);
         wake->SetAlpha(170);
         m->trailActive = true;
      }
   } else {
      ec->SetEntityState(CigiEntityCtrlV3::Standby);
      wake->SetEntityState(CigiEntityCtrlV3::Standby);
   }

   return true;
}

//------------------------------------------------------------------------------
// setSpaceVehicleData()
//  -- Sets a CigiEntityCtrlV3 structure to a space vheicle's state
//------------------------------------------------------------------------------
bool CigiHost::setSpaceVehicleData(CigiModel* const m, const int entity, const models::SpaceVehicle* const p)
{
   // Make sure we have an entity control block
   if (m->parentEC[iw] == nullptr) {
      m->parentEC[iw] = new CigiEntityCtrlV3();
   }
   CigiEntityCtrlV3* const ec{m->parentEC[iw]};

   // As long as we're active, update the entity control
   if (m->getState() == CigiModel::State::ACTIVE) {
      bool ok{setCommonModelData(ec, entity, p)};
      if (ok) {
         // Set the entity type
         int tt{};
         const Player2CigiMap* otm{m->getTypeMapper()};
         if (otm != nullptr) tt = otm->getEntityId();
         if (tt > 0xffff) tt = 0;
         ec->SetEntityType(static_cast<Cigi_uint16>(tt));

         // Set entity control data
         ec->SetEntityState(CigiEntityCtrlV3::Active);
         ec->SetAttachState(CigiEntityCtrlV3::Detach);
         ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
         ec->SetAnimationState(CigiEntityCtrlV3::Stop);
         ec->SetAlpha(255);
      }
      m->parentActive = ok;
   } else {
      ec->SetEntityState(CigiEntityCtrlV3::Standby);
   }
   return true;
}

//------------------------------------------------------------------------------
// setWeaponData() -- Sets a 'model_t' structure to a weapon's state
//------------------------------------------------------------------------------
bool CigiHost::setWeaponData(CigiModel* const m, const int entity, const models::AbstractWeapon* const p)
{
   // Make sure we have an entity control block, ...
   if (m->parentEC[iw] == nullptr) {
      m->parentEC[iw] = new CigiEntityCtrlV3();
      m->parentActive = false;
   }
   CigiEntityCtrlV3* const ec{m->parentEC[iw]};

   // an ground explosion entity control block, ...
   if (m->explosionEC[iw] == nullptr) {
      m->explosionEC[iw] = new CigiEntityCtrlV3();
      m->explosionEC[iw]->SetEntityID(entity+2); // Explosions are at main entity_id plus two
      m->explosionEC[iw]->SetParentID(entity);

      m->explosionEC[iw]->SetRoll(0.0f);
      m->explosionEC[iw]->SetPitch(0.0f);
      m->explosionEC[iw]->SetYaw(0.0f);
      m->explosionEC[iw]->SetAlt(0.0f);
      m->explosionEC[iw]->SetLat(0.0);
      m->explosionEC[iw]->SetLon(0.0);

      m->explosionEC[iw]->SetEntityState(CigiEntityCtrlV3::Active);
      m->explosionEC[iw]->SetEntityType(static_cast<Cigi_uint16>(cmtAirExplosion));
      m->explosionEC[iw]->SetAttachState(CigiEntityCtrlV3::Detach);
      m->explosionEC[iw]->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
      m->explosionEC[iw]->SetAnimationState(CigiEntityCtrlV3::Stop);
      m->explosionEC[iw]->SetAlpha(255);
      m->explosionActive = true;  // True the first time to make sure it's loaded

      //if (isMessageEnabled(MSG_DEBUG)) {
      //std::cout << "explosion: create" << m->explosionEC[iw]->entity_id << std::endl;
      //}
   }
   CigiEntityCtrlV3* const explosion{m->explosionEC[iw]};

   // As long as we're active, update the entity control
   if (m->getState() == CigiModel::State::ACTIVE) {
      // Load the parent entity control
      setCommonModelData(ec, entity, p);

      // Set the entity type
      int tt{};
      const Player2CigiMap* otm{m->getTypeMapper()};
      if (otm != nullptr) tt = otm->getEntityId();
      if (tt > 0xffff) tt = 0;
      ec->SetEntityType(static_cast<Cigi_uint16>(tt));

      // Set entity control data
      ec->SetEntityState(CigiEntityCtrlV3::Active);
      ec->SetAttachState(CigiEntityCtrlV3::Detach);
      ec->SetCollisionDetectEn(CigiEntityCtrlV3::Disable);
      ec->SetAnimationState(CigiEntityCtrlV3::Stop);
      ec->SetAlpha(255);
      m->parentActive = true;
   } else {
      ec->SetEntityState(CigiEntityCtrlV3::Standby);
      if (m->getState() == CigiModel::State::DEAD) {
         // Start air explosion at last known location of missile
         explosion->SetRoll(ec->GetRoll());
         explosion->SetPitch(ec->GetPitch());
         float yaw{ec->GetYaw()};
         if (yaw < 0.0) yaw += 360.0f;
         if (yaw >= 360.0f) yaw = 0.0;
         explosion->SetYaw(yaw, false);
         if (ec->GetAlt() > p->getTerrainElevation())
            explosion->SetAlt(ec->GetAlt());
         else
            explosion->SetAlt(p->getTerrainElevation());
         explosion->SetLat(ec->GetLat());
         explosion->SetLon(ec->GetLon());
         //explosion->SetAnimationState(CigiEntityCtrlV3::Continue);
         explosion->SetAnimationState(CigiEntityCtrlV3::Play);
         m->explosionActive = true;  // True to start to sequence
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// Line of sight request -- returns true of request was issued
//------------------------------------------------------------------------------
bool CigiHost::lineOfSightRequest(
                                   const double lat,          // Source latitude         (deg)
                                   const double lon,          // Source longitude        (deg)
                                   const double alt,          // Source altitude         (m)
                                   const double hdg,          // Source heading          (deg)
                                   const double pitch,        // Source pitch            (deg)
                                   const double minRange,     // Request minimum range   (m)
                                   const double maxRange      // Request maximum range   (m)
                                   )
{
   bool ok{};
   if ( !isLosRequestPending() ) {
      CigiLosVectReqV3* losReq{getLosRangeRequestPacket()};
      if (losReq != nullptr) {
         losReq->SetSrcLat(lat);
         losReq->SetSrcLon(lon);
         losReq->SetSrcAlt(alt);
         losReq->SetVectAz( static_cast<float>(-hdg) );
         losReq->SetVectEl( static_cast<float>(pitch) );
         losReq->SetMinRange( static_cast<float>(minRange) );
         losReq->SetMaxRange( static_cast<float>(maxRange) );
         newLosReq = true;             // new request,
         losRespDataValid = false;     // so, the previous data is not valid
         ok = true;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Get Line of sight -- returns true of the LOS data is valid
//------------------------------------------------------------------------------
bool CigiHost::getLineOfSightData(
                                   double* const lat,      // Point latitude         (deg)
                                   double* const lon,      // Point longitude        (deg)
                                   double* const alt,      // Point altitude         (m)
                                   double* const rng,      // Range to point         (m)
                                   int* const material     // Material code
                                   )
{
   bool ok{losRespDataValid};
   if (ok) {
      if (lat != nullptr) *lat = losRespLat;
      if (lon != nullptr) *lon = losRespLon;
      if (alt != nullptr) *alt = losRespAlt;
      if (rng != nullptr) *rng = losRespRange;
      if (material != nullptr) *material = 0;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Set Slot Functions
//------------------------------------------------------------------------------

bool CigiHost::setSlotHostSession(HostSession* const msg)
{
   session = msg;
   return true;
}

// Set/clear the ASYNC mode
bool CigiHost::setSlotASyncMode(const base::Boolean* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      setASyncMode(msg->getBoolean());
      ok = true;
   }
   return ok;
}

// Set/clear the hide ownship model flag
bool CigiHost::setSlotHideOwnshipModel(const base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      setHideOwnshipModel(msg->getBoolean());
      ok = true;
   }
   return ok;
}

bool CigiHost::setSlotOwnshipModelId(const base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int id{msg->getInt()};
      if (id >= 0 && id <= 0xFFFF) {
         setOwnshipModelId(id);
         ok = true;
      }
   }
   return ok;
}

bool CigiHost::setSlotMslTrailModelId(const base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int id{msg->getInt()};
      if (id >= 0 && id <= 0xFFFF) {
         setMslTrailModelId(id);
         ok = true;
      }
   }
   return ok;
}

bool CigiHost::setSlotSmokePlumeModelId(const base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int id{msg->getInt()};
      if (id >= 0 && id <= 0xFFFF) {
         setSmokePlumeModelId(id);
         ok = true;
      }
   }
   return ok;
}

bool CigiHost::setSlotAirExplosionModelId(const base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int id{msg->getInt()};
      if (id >= 0 && id <= 0xFFFF) {
         setAirExplosionModelId(id);
         ok = true;
      }
   }
   return ok;
}

bool CigiHost::setSlotGroundExplosionModelId(const base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int id{msg->getInt()};
      if (id >= 0 && id <= 0xFFFF) {
         setGroundExplosionModelId(id);
         ok = true;
      }
   }
   return ok;
}

bool CigiHost::setSlotShipWakeModelId(const base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int id{msg->getInt()};
      if (id >= 0 && id <= 0xFFFF) {
         setShipWakeModelId(id);
         ok = true;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// sendCigiData() -- Handles sending CIGI data to the visual system
//------------------------------------------------------------------------------
bool CigiHost::sendCigiData()
{
   // Select the 'read' buffer index
   if (iw0 == NUM_BUFFERS) return false;
   ir = iw0;

   // ---
   // Start the message.  This MUST come before any CigiAddPacket*()
   // functions.
   // ---
   session->startMessage();

   // ---
   // Now we must add an IG Control.
   // ---
   CigiIGCtrlV3* ig_cp{getIgControlPacket()};
   ig_cp->SetDatabaseID(0);
   ig_cp->SetIGMode(CigiIGCtrlV3::Operate);
   if (resetRequest) {
      ig_cp->SetIGMode(CigiIGCtrlV3::Standby);
      resetRequest = false;
   }
   ig_cp->SetFrameCntr(ig_cp->GetFrameCntr() + 1);
   ig_cp->SetTimeStamp(0);
   session->addPacketIGCtrl(ig_cp);

   // ---
   // And add an Entity Control packet for the "ownship."
   // ---
   session->addPacketEntityCtrl(getOwnshipEntityControlPacket(ir));
   session->addPacketComponentCtrl(getOwnshipComponentControlPacket(ir));

   // ---
   // Send all entity controls from the model table
   // ---
   {
      // Do we have models?
      CigiModel** const table{reinterpret_cast<CigiModel**>(getModelTable())};
      if (table != nullptr && getModelTableSize() > 0) {

         // Need at least this much in the buffer to add another entity
         int padding{(sizeof(CigiEntityCtrlV3) * 2) + sizeof(CigiHatHotReqV3)};
         if (newLosReq && getLosRangeRequestPacket() != nullptr) padding += sizeof(CigiLosVectReqV3);
         if (getViewControlPacket() != nullptr) padding += sizeof(CigiViewCtrlV3);
         if (getViewDefinitionPacket() != nullptr) padding += sizeof(CigiViewDefV3);

         // Add all of the models (that we can) to the buffer.
         int sendSize{session->getOutgoingBufferSize()};
         int maxAge{static_cast<int>(getModelTableSize())};
         for (int i = 0; i < getModelTableSize() && sendSize < (MAX_BUF_SIZE - padding); i++) {
            base::safe_ptr<CigiModel> model( static_cast<CigiModel*>(table[i]) );
            if (model != nullptr) {

               // For all active models in the table ...
               // (send ground models only after 'maxAge' frames)
               // (always update the inactive models to clear them)
               model->incAgeCount();
               bool updateThisOne{!model->isGroundPlayer || model->isState(CigiModel::State::OUT_OF_RANGE) || (model->isGroundPlayer && model->getAgeCount() >= maxAge)};

               if (updateThisOne) {
                  //if (model->isGroundPlayer) {
                  //   if (isMessageEnabled(MSG_DEBUG)) {
                  //   std::cout << "model: " << model->getID() << ", pid=" << model->getPlayer()->getID() << std::endl;
                  //}
                  //}
                  // Explosion?
                  if (model->explosionActive && model->explosionEC[ir] != nullptr) {
                     session->addPacketEntityCtrl(model->explosionEC[ir]);
                     model->explosionActive = false;
                  }

                  if (model->parentActive && model->parentEC[ir] != nullptr) {
                     session->addPacketEntityCtrl(model->parentEC[ir]);
                     model->parentActive = (model->parentEC[ir]->GetEntityState() == CigiEntityCtrlV3::Active);

                     // Trail effect?
                     if (model->trailActive && model->trailEC[ir] != nullptr) {
                        session->addPacketEntityCtrl(model->trailEC[ir]);
                        model->trailActive = (model->trailEC[ir]->GetEntityState() == CigiEntityCtrlV3::Active);
                     }

                     // Smoke affect?
                     if (model->smokeActive && model->smokeEC[ir] != nullptr) {
                        session->addPacketEntityCtrl(model->smokeEC[ir]);
                        model->smokeActive = (model->smokeEC[ir]->GetEntityState() == CigiEntityCtrlV3::Active);
                     }

                     // Animation state?
                     if (model->animationActive && model->animationCC[ir] != nullptr) {
                        session->addPacketComponentCtrl(model->animationCC[ir]);
                        model->animationActive = (model->animationCC[ir]->GetCompState() > 0);
                     }

                     // Damage state?
                     if (model->damageActive && model->damageCC[ir] != nullptr) {
                        session->addPacketComponentCtrl(model->damageCC[ir]);
                        model->damageActive = (model->damageCC[ir]->GetCompState() > 1);
                     }

                     // Launcher articulated state?
                     if (model->launcherApcActive && model->launcherAPC[ir] != nullptr) {
                        session->addPacketArtPartCtrl(model->launcherAPC[ir]);
                        model->launcherApcActive = (model->launcherAPC[ir]->GetPitchEn());
                     }

                     // Attached part?
                     if (model->attachedEcActive && model->attachedEC[ir] != nullptr) {
                        session->addPacketEntityCtrl(model->attachedEC[ir]);
                        model->attachedEcActive = (model->attachedEC[ir]->GetEntityState() == CigiEntityCtrlV3::Active);
                     }

                     // Attached part component control?
                     if (model->attachedCcActive && model->attachedCC[ir] != nullptr && model->attachedEC[ir] != nullptr) {
                        session->addPacketComponentCtrl(model->attachedCC[ir]);
                        // we come and go with the attached part
                        model->attachedCcActive = (model->attachedEC[ir]->GetEntityState() == CigiEntityCtrlV3::Active);
                     }

                     // Clear the model?
                     if (model->getState() != CigiModel::State::ACTIVE) {
                        model->setState( CigiModel::State::CLEARED );
                     }
                  }
                  model->setAgeCount(0);
                  sendSize = session->getOutgoingBufferSize();
               }
            }
         }
      }
   }

   // ---
   // Send all elevation (Height-Of-Terrain) requests
   //   (only if we don't have an outstanding request)
   // ---
   if (!isElevationRequestPending()) {
      // Do we have a elevation request entries?
      CigiModel** const table{reinterpret_cast<CigiModel**>( getElevationTable() )};
      if (table != nullptr && getElevationTableSize() > 0) {
         int maxAge{getElevationTableSize() * 4};

         // For all active elevation requests in the table ...
         // -- look for the oldest request ---
         base::safe_ptr<CigiModel> oldest;
         base::safe_ptr<CigiModel> model;
         for (int i = 0; i < getElevationTableSize(); i++) {
            model = table[i];
            if (model != nullptr) {
               // Must be active and haven't been requested for at least TBD frames ...
               if (model->isHotActive() && model->getReqCount() >= maxAge) {
                  if (oldest == nullptr) oldest = model;
                  else if (model->getReqCount() > oldest->getReqCount()) oldest = model;
               }
            }
         }
         model = nullptr;

         if (oldest != nullptr) {

            int idx{-1};
            for (int i = 0; idx < 0 && i < getElevationTableSize(); i++) {
               if (table[i] == oldest) idx = i;
            }

            if (idx >= 0) {
               CigiHatHotReqV3 hotRequest;
               hotRequest.SetHatHotID(table[idx]->getID());

               // Requested Position (lat/lon)
               double hotLat{}, hotLon{};
               dynamic_cast<models::Player*>(oldest->getPlayer())->getPositionLL(&hotLat, &hotLon);
               hotRequest.SetLat(hotLat);
               hotRequest.SetLon(hotLon);
               hotRequest.SetReqType(CigiHatHotReqV3::HOT);
               //base::Vec3 pos = oldest->getPlayer()->getPosition();
               //double alt;
               //base::nav::convertPosVec2LL(
               //         getRefLatitude(), getRefLongitude(),
               //         pos,
               //         &hotRequest.lat, &hotRequest.lon, &alt);

               session->addPacketHatHotReq(&hotRequest);
               oldest->setReqCount(0);

               elevationRequestSend();
            }
            oldest = nullptr;
         }
      }
   }

   // ---
   // Optional LOS request packet
   // ---
   {
      CigiLosVectReqV3* los0{getLosRangeRequestPacket()};
      if (isNewLosequested() && los0 != nullptr) {
         los->SetLosID(getNexLosId());
         session->addPacketLosRangeReq(los0);
         losRequestSend();
      }
   }

   // ---
   // Optional view control and definition packets
   // ---
   if (getSensorControlPacket() != nullptr) {
      CigiSensorCtrlV3* mySensor{getSensorControlPacket()};
      session->addPacketSensorCtrl(mySensor);
   }

   // ---
   // Optional view control and definition packets
   // ---
   if (getViewControlPacket() != nullptr) {
      CigiViewCtrlV3* myView{getViewControlPacket()};
      //if ( getMaxModels() == 20 ) {
      //if (isMessageEnabled(MSG_DEBUG)) {
      //std::cout << myView->entity_id << std::endl;
      //std::cout << myView->view_id << std::endl;
      //std::cout << myView->group << std::endl;
      //std::cout << myView->heading_enable << std::endl;
      //std::cout << myView->pitch_enable << std::endl;
      //std::cout << myView->roll_enable << std::endl;
      //std::cout << myView->xoff_enable << std::endl;
      //std::cout << myView->yoff_enable << std::endl;
      //std::cout << myView->zoff_enable << std::endl;
      //std::cout << myView->x_offset << std::endl;
      //std::cout << myView->y_offset << std::endl;
      //std::cout << myView->z_offset << std::endl;
      //std::cout << myView->heading_offset << std::endl;
      //std::cout << myView->pitch_offset << std::endl;
      //std::cout << myView->roll_offset << std::endl;
      //}
      //}

      session->addPacketViewCtrl(myView);
   }
   if (getViewDefinitionPacket() != nullptr) {
      session->addPacketViewDef(getViewDefinitionPacket());
      //CigiViewDefV3* myView = getViewDefinitionPacket();
      //if (isMessageEnabled(MSG_DEBUG)) {
      //std::cout << "VIEW DEFINITION PACKET PARAMETERS: " << std::endl;
      //std::cout << "Group = " << myView->group << std::endl;
      //std::cout << "Mirror = " << myView->mirror << std::endl;
      //std::cout << "Track Assign = " << myView->track_assign << std::endl;
      //std::cout << "Type = " << myView->type << std::endl;
      //std::cout << "View ID = " << myView->view_id << std::endl;
      //std::cout << "Bottom enable = " << myView->bottom_enable << std::endl;
      //std::cout << "Top enable = " << myView->top_enable << std::endl;
      //std::cout << "Left enable = " << myView->left_enable << std::endl;
      //std::cout << "Right enable = " << myView->right_enable << std::endl;
      //std::cout << "Near Enabled = " << myView->near_enable << std::endl;
      //std::cout << "Far enabled = " << myView->far_enable << std::endl;
      //std::cout << "Far = " << myView->fov_far << std::endl;
      //std::cout << "Near = " << myView->fov_near << std::endl;
      //std::cout << "Left = " << myView->fov_left << std::endl;
      //std::cout << "Right = " << myView->fov_right << std::endl;
      //std::cout << "Top = " << myView->fov_top << std::endl;
      //std::cout << "Bottom = " << myView->fov_bottom << std::endl;
      //}

   }

   // ---
   // End the message.
   // ---
   session->endMessage();

   return true;
}

//------------------------------------------------------------------------------
// startOfFrame() -- Handles Start of Frame packets
//------------------------------------------------------------------------------
void CigiHost::startOfFrame(const CigiSOFV3* const)
{
   if (isSyncMode()) sendCigiData();
}

//------------------------------------------------------------------------------
// losResp() -- Handles Line-Of-Sight Response packets
//------------------------------------------------------------------------------
void CigiHost::losResp(const CigiLosRespV3* const p)
{
   if ( p != nullptr && isLosRequestPending() && losReqTimer > 0.0 ) {
      // We do have a pending request that hasn't timed out,
      // but is this it?
      if (p->GetLosID() == losReqId) {
         // We've the packet that we're looking for,
         // but is it valid (the IG found an intersection point)
         if (p->GetValid()) {
            // We have valid data
            losRespDataValid = true;
            losRespLat = p->GetLatitude();
            losRespLon = p->GetLongitude();
            losRespAlt = p->GetAltitude();
            losRespRange = static_cast<double>(p->GetRange());
         } else {
            // Don't have a valid point
            losRespDataValid = false;
         }
         losRespId = p->GetLosID();
         losReqTimer = 0;
      }
   }
}

//------------------------------------------------------------------------------
// LOS request has been sent to the IG
//------------------------------------------------------------------------------
void CigiHost::losRequestSend()
{
   newLosReq = false;
   losReqTimer = LOS_REQ_TIMEOUT;
}

//------------------------------------------------------------------------------
// Elevation request has been sent to the IG
//------------------------------------------------------------------------------
void CigiHost::elevationRequestSend()
{
   elevReqFlg = true;   // active request
   elevReqTimer = 0.5;  // time-out
}

//------------------------------------------------------------------------------
// collisionSegmentResp() -- Handles Collision Segment Response packets
//------------------------------------------------------------------------------
void CigiHost::collisionSegmentResp(const CigiCollDetSegRespV3* const p)
{
   if (p != nullptr) {
      std::cout << "<< Incoming\n";
#if 0
      CigiPrintCollisionSegmentRespPacket(p, 2);
#endif
      std::cout << "\n";
   }
}

//------------------------------------------------------------------------------
// sensorResp() -- Handles Sensor Response packets
//------------------------------------------------------------------------------
void CigiHost::sensorResp(const CigiSensorRespV3* const p)
{
   if (p != nullptr) {
      std::cout << "<< Incoming\n";
#if 0
      CigiPrintSensorResponsePacket(p, 2);
#endif
      std::cout << "\n";
   }
}

//------------------------------------------------------------------------------
// hatHotResp() -- Handles Height-Above-Terrain and Height-Of-Terrain Response packets
//------------------------------------------------------------------------------
void CigiHost::hatHotResp(const CigiHatHotRespV3* const p)
{
   // Valid?
   if (p != nullptr && p->GetValid()) {
      // Yes and the hot_id is the elevation table index
      int id{p->GetHatHotID()};

      //if (isMessageEnabled(MSG_DEBUG)) {
      //std::cout << "hotResp: id = " << id << std::endl;
      //}

      CigiModel** const table{reinterpret_cast<CigiModel**>( getElevationTable() )};
      base::safe_ptr<CigiModel> model(nullptr);
      for (int i = 0; i < getElevationTableSize() && model == nullptr; i++) {
         if (table[i]->getID() == id) model = table[i];
      }
      if (model != nullptr) {
         if (model->isHotActive() && model->getPlayer() != nullptr) {
            // When the player and elevation table are still valid, store
            // the terrain elevation (meters)
            dynamic_cast<models::Player*>(model->getPlayer())->setTerrainElevation(static_cast<double>(p->GetHot()));

            //if (isMessageEnabled(MSG_DEBUG)) {
            //   std::cout << "hotResp: alt = --, pid = " << model->getPlayer()->getID() << std::endl;
            //}
         }
      }
      elevReqFlg = false;
      elevReqTimer = 0.0;
   }
}

//------------------------------------------------------------------------------
// collisionVolumeResp() -- Handles Collision Volume Response packets
//------------------------------------------------------------------------------
void CigiHost::collisionVolumeResp(const CigiCollDetVolRespV3* const p)
{
   if (p != nullptr) {
      std::cout << "<< Incoming\n";
#if 0
      CigiPrintCollisionVolumeRespPacket(p, 2);
#endif
      std::cout << "\n";
   }
}

//------------------------------------------------------------------------------
// igResponse() -- Handles IG Response packets
//------------------------------------------------------------------------------
void CigiHost::igResponse(const CigiIGMsgV3* const p)
{
   if (p != nullptr) {
      std::cout << "<< Incoming\n";
#if 0
      CigiPrintIGResponseMessagePacket(p, 2);
#endif
      std::cout << "\n";
   }
}

}
}
