
#include "mixr/models/Actions.hpp"

#include "mixr/models/player/weapon/Bomb.hpp"
#include "mixr/models/player/Player.hpp"
#include "mixr/models/system/OnboardComputer.hpp"
#include "mixr/models/system/Sar.hpp"
#include "mixr/models/system/StoresMgr.hpp"
#include "mixr/models/navigation/Steerpoint.hpp"

#include "mixr/models/WorldModel.hpp"

#include "mixr/base/Latitude.hpp"
#include "mixr/base/Longitude.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/units/lengths.hpp"
#include "mixr/base/util/nav_utils.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Action, "Action")
EMPTY_SLOTTABLE(Action)

Action::Action()
{
   STANDARD_CONSTRUCTOR()
}

void Action::copyData(const Action& org, const bool)
{
   BaseClass::copyData(org);

   refId = org.refId;
   completed = org.completed;

   setManager(nullptr);
}

void Action::deleteData()
{
   setManager(nullptr);
}

//------------------------------------------------------------------------------
// Data access functions
//------------------------------------------------------------------------------

// True if action is ready to start
bool Action::isReadyToStart()
{
   return !isInProgress() && !isCompleted();
}

// True if action is in progress
bool Action::isInProgress()
{
   return (manager != nullptr);
}

// True if action has been completed
bool Action::isCompleted()
{
   return completed;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the completed flag
void Action::setCompleted(const bool flg)
{
   if (flg) setManager(nullptr);
   completed = flg;
}

// Sets the message ref ID
void Action::setRefId(const int id)
{
   refId = id;
}

// Set our route manager
bool Action::setManager(OnboardComputer* const mgr)
{
   manager = mgr;
   return true;
}

//------------------------------------------------------------------------------
// Starts this action
//------------------------------------------------------------------------------
bool Action::trigger(OnboardComputer* const mgr)
{
   setManager(mgr);
   completed = false;
   return true;
}

//------------------------------------------------------------------------------
// Cancels this action
//------------------------------------------------------------------------------
bool Action::cancel()
{
   completed = true;
   setManager(nullptr);
   return true;
}

//------------------------------------------------------------------------------
// process() -- action processing
//------------------------------------------------------------------------------
void Action::process(const double)
{
}

//------------------------------------------------------------------------------
// Execute as an UBF action
// -- Use the 'actor' to find our OBC and 'trigger' this action.
//------------------------------------------------------------------------------
bool Action::execute(base::Component* actor)
{
   bool ok{};
   if (actor != nullptr) {

      // Was the actor our OBC?
      auto obc = dynamic_cast<OnboardComputer*>( actor );

      // If not, was it our ownship ...
      //   and can we get our OBC from our ownship
      if (obc == nullptr) {
         const auto own = dynamic_cast<Player*>( actor );
         if (own != nullptr) {
            obc = own->getOnboardComputer();
         }
      }

      // If we could find our OBC from the actor then trigger this action
      if (obc != nullptr) {
         ok = trigger(obc);
      }

   }
   return ok;
}


// =============================================================================
// Class: ActionImagingSar
// =============================================================================
IMPLEMENT_SUBCLASS(ActionImagingSar,"ActionImagingSar")

BEGIN_SLOTTABLE(ActionImagingSar)
   "sarLatitude",    //  1) Target's latitude
   "sarLongitude",   //  2) Target's longitude
   "sarElevation",   //  3) Target's elevation (Length) (default: 0)
   "resolution",     //  4) Image (pixel) resolution   (Length) (default: 1 meter)
   "imageSize",      //  5) Image size: height & width (pixels) (default: 512)
END_SLOTTABLE(ActionImagingSar)

BEGIN_SLOT_MAP(ActionImagingSar)
    ON_SLOT(1, setSlotSarLat,     base::Latitude)
    ON_SLOT(2, setSlotSarLon,     base::Longitude)
    ON_SLOT(3, setSlotSarElev,    base::Length)
    ON_SLOT(4, setSlotResolution, base::Length)
    ON_SLOT(5, setSlotImageSize,  base::Integer)
END_SLOT_MAP()

ActionImagingSar::ActionImagingSar()
{
   STANDARD_CONSTRUCTOR()
}

void ActionImagingSar::copyData(const ActionImagingSar& org, const bool)
{
   BaseClass::copyData(org);

   sarLatitude = org.sarLatitude;
   sarLongitude = org.sarLongitude;
   sarElevation = org.sarElevation;
   resolution = org.resolution;
   imgSize = org.imgSize;
   orientation = org.orientation;

   timer = 0.0;
   setSarSystem(nullptr);
}

void ActionImagingSar::deleteData()
{
}

//------------------------------------------------------------------------------
// Default function: Start the SAR capture, ATR run and Image download process
//------------------------------------------------------------------------------
bool ActionImagingSar::trigger(OnboardComputer* const mgr)
{
   bool ok{};

   if (mgr != nullptr) {

      // Find our ownship player & SAR system
      Player* ownship{static_cast<Player*>(mgr->findContainerByType(typeid(Player)))};
      if (ownship != nullptr) {
         base::Pair* pair{ownship->getSensorByType(typeid(Sar))};
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "Looking for SAR: pair = " << pair << std::endl;
         }
         if (pair != nullptr) {
            setSarSystem( static_cast<Sar*>(pair->object()) );
         }
      }

      if (sar != nullptr && ownship != nullptr) {
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "Requesting an image from the SAR: refId: " << getRefId() << std::endl;
         }
         sar->setStarePoint( getSarLatitude(), getSarLongitude(), static_cast<double>(getSarElevation()) );
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "And resolution: " << getResolution() << std::endl;
         }
         sar->requestImage(getImageSize(), getImageSize(), getResolution() );
         ok = true;
      }

      timer = 0;
   }

   if (ok) setManager(mgr);
   else setManager(nullptr);

   return ok;
}

//------------------------------------------------------------------------------
// Cancels this action
//------------------------------------------------------------------------------
bool ActionImagingSar::cancel()
{
   if (getSarSystem() != nullptr) {
      getSarSystem()->cancel();
   }
   return BaseClass::cancel();
}

//------------------------------------------------------------------------------
// process() -- action processing
//------------------------------------------------------------------------------
void ActionImagingSar::process(const double dt)
{
   BaseClass::process(dt);

   if (isInProgress() && getSarSystem() != nullptr) {

      // Check if SAR has finished
      if ( !getSarSystem()->isImagingInProgress()) {
         // Imaging is NOT in progress, so it must have completed ...
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "Processing SAR -- completed!" << std::endl;
         }
         setCompleted(true);
      }

      // Process safety time-out
      timer += dt;
      static const double MAX_SAR_TIME{120.0};
      if (timer > MAX_SAR_TIME) {
         // Cancel the SAR
         cancel();
         setCompleted(true);
      }
   }
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the SAR latitude in degrees
bool ActionImagingSar::setSarLatitude(const double v)
{
   bool ok{};
   if (v >= -90.0 && v <= 90.0) {
      sarLatitude = v;
      ok = true;
   }
   return ok;
}

// Sets the SAR longitude in degrees
bool ActionImagingSar::setSarLongitude(const double v)
{
   bool ok{};
   if (v >= -180.0 && v <= 180.0) {
      sarLongitude = v;
      ok = true;
   }
   return ok;
}

// Sets the SAR elevation in meters
bool ActionImagingSar::setSarElevation(const double v)
{
   sarElevation = v;
   return true;
}

// Sets the SAR image resolution (meters)
bool ActionImagingSar::setResolution(const double r)
{
   resolution = r;
   return true;
}

// Sets the image size
bool ActionImagingSar::setImageSize(const unsigned int v)
{
   bool ok{};
   if (v == 512 || v == 1024 || v == 2048 || v == 4096) {
      imgSize = v;
      ok = true;
   }
   return ok;
}

// Sets the completed flag
void ActionImagingSar::setCompleted(const bool flg)
{
   if (flg) setSarSystem(nullptr);
   BaseClass::setCompleted(flg);
}

// Set the SAR system
void ActionImagingSar::setSarSystem(Sar* const p)
{
   if (sar != nullptr) {
      sar->unref();
   }
   sar = p;
   if (sar != nullptr) {
      sar->ref();
   }
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

bool ActionImagingSar::setSlotSarLat(const base::Latitude* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setSarLatitude(x->getDecimalDegrees());
   }
   return ok;
}

bool ActionImagingSar::setSlotSarLon(const base::Longitude* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setSarLongitude(x->getDecimalDegrees());
   }
   return ok;
}

bool ActionImagingSar::setSlotSarElev(const base::Length* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setSarElevation(x->getValueInMeters());
   }
   return ok;
}

bool ActionImagingSar::setSlotResolution(const base::Length* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setResolution(x->getValueInMeters());
   }
   return ok;
}

bool ActionImagingSar::setSlotImageSize(const base::Integer* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setImageSize(x->asInt());
      if (!ok) {
         if (isMessageEnabled(MSG_INFO)) {
         std::cout << "  ActionImagingSar::setSlotImageSize must be 512, 1024, 2048 or 4096, not " << x->asInt() << std::endl;
      }
   }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Computes the planned image orientation (degs)
//------------------------------------------------------------------------------
double ActionImagingSar::computeOrientation(const Steerpoint* const wp)
{
   if (wp != nullptr) {
      double slat{wp->getLatitude()};
      double slon{wp->getLongitude()};
      double dlat{getSarLatitude()};
      double dlon{getSarLongitude()};
      double brg{}, distNM{};
      base::nav::gll2bd(slat, slon, dlat, dlon, &brg, &distNM);
      orientation = static_cast<double>(-brg);
   }
   return orientation;
}

// =============================================================================
// Class: ActionWeaponRelease
// =============================================================================

IMPLEMENT_SUBCLASS(ActionWeaponRelease,"ActionWeaponRelease")
EMPTY_DELETEDATA(ActionWeaponRelease)

BEGIN_SLOTTABLE(ActionWeaponRelease)
   "targetLatitude",    //  1) Target's latitude
   "targetLongitude",   //  2) Target's longitude
   "targetElevation",   //  3) Target's elevation (feet)
   "station",           //  4) Station number to use (default to next available)
END_SLOTTABLE(ActionWeaponRelease)

BEGIN_SLOT_MAP(ActionWeaponRelease)
    ON_SLOT(1, setSlotTargetLat,  base::Latitude)
    ON_SLOT(2, setSlotTargetLon,  base::Longitude)
    ON_SLOT(3, setSlotTargetElev, base::Number)
    ON_SLOT(4, setSlotStationNum, base::Integer)
END_SLOT_MAP()

ActionWeaponRelease::ActionWeaponRelease()
{
   STANDARD_CONSTRUCTOR()
}

void ActionWeaponRelease::copyData(const ActionWeaponRelease& org, const bool)
{
   BaseClass::copyData(org);

   station = org.station;
   targetLatitude = org.targetLatitude;
   targetLongitude = org.targetLongitude;
   targetElevation = org.targetElevation;
}

//------------------------------------------------------------------------------
// Default function: Launch a weapon
//------------------------------------------------------------------------------
bool ActionWeaponRelease::trigger(OnboardComputer* const mgr)
{
   bool ok{};

   if (mgr != nullptr) {
      Player* own{static_cast<Player*>(mgr->findContainerByType(typeid(Player)))};
      if (own != nullptr) {

         StoresMgr* sms{own->getStoresManagement()};
         if (sms != nullptr) {

            // Get the simulation ref point
            WorldModel* sim{own->getWorldModel()};
            const double refLat{sim->getRefLatitude()};
            const double refLon{sim->getRefLongitude()};

            // Computes the target's position vector
            base::Vec3d tgtPos;                          // Target position  (m) NED
            base::nav::convertLL2PosVec(
               refLat, refLon,                           // Ref point (at sea level)
               getTargetLatitude(), getTargetLongitude(), getTargetElevation(),
               &tgtPos); // x,y,z  NED

            if (isMessageEnabled(MSG_INFO)) {
               std::cout << "ActionWeaponRelease::trigger() Release weapon at [ " << tgtPos[0] << ", } " << tgtPos[1] << " ]" << std::endl;
            }

            // Release the weapon and set the target
            AbstractWeapon* flyout{sms->releaseOneBomb()};
            if (flyout != nullptr) {
                flyout->setTargetPosition(tgtPos);
            }
         }

         BaseClass::setCompleted(true);
         ok = true;
      }
   }

   return ok;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the target location (latitude & longitude in radians, elevation in meters)
bool ActionWeaponRelease::setTargetLocation(const double tgtLat, const double tgtLon, const double tgtElev)
{
   targetLatitude = tgtLat;
   targetLongitude = tgtLon;
   targetElevation = tgtElev;
   return true;
}

// Set planned station number
bool ActionWeaponRelease::setStation(const unsigned int num)
{
   station = num;
   return true;
}

// Slot functions
bool ActionWeaponRelease::setSlotTargetLat(const base::Latitude* newLat)
{
    bool ok{};
    if (newLat != nullptr) {
        targetLatitude = newLat->getDecimalDegrees();
        ok = true;
    }
    return ok;
}
bool ActionWeaponRelease::setSlotTargetLon(const base::Longitude* newLon)
{
    bool ok{};
    if (newLon != nullptr) {
        targetLongitude = newLon->getDecimalDegrees();
        ok = true;
    }
    return ok;
}
bool ActionWeaponRelease::setSlotTargetElev(const base::Number* newElev)
{
    bool ok{};
    if (newElev != nullptr) {
        targetElevation = newElev->asDouble();
        ok = true;
    }
    return ok;
}
bool ActionWeaponRelease::setSlotStationNum(const base::Integer* newStation)
{
    bool ok{};
    if (newStation != nullptr) {
        station = newStation->asInt();
        ok = true;
    }
    return ok;
}

// =============================================================================
// Class: ActionDecoyRelease
// =============================================================================
IMPLEMENT_SUBCLASS(ActionDecoyRelease, "ActionDecoyRelease")
EMPTY_DELETEDATA(ActionDecoyRelease)

BEGIN_SLOTTABLE(ActionDecoyRelease)
   "numToLaunch",    //  1) number of decoys to launch
   "interval",       //  2) time, in seconds, between launches
END_SLOTTABLE(ActionDecoyRelease)

BEGIN_SLOT_MAP(ActionDecoyRelease)
    ON_SLOT(1, setSlotNumToLaunch, base::Integer)
    ON_SLOT(2, setSlotInterval,    base::Number)
END_SLOT_MAP()

ActionDecoyRelease::ActionDecoyRelease()
{
   STANDARD_CONSTRUCTOR()
}

void ActionDecoyRelease::copyData(const ActionDecoyRelease& org, const bool)
{
   BaseClass::copyData(org);
   interval = org.interval;
   numToLaunch = org.numToLaunch;
   tod = org.tod;
   startTOD = org.startTOD;
}

//------------------------------------------------------------------------------
//Default function: Launch a decoy
//------------------------------------------------------------------------------
bool ActionDecoyRelease::trigger(OnboardComputer* const mgr)
{
   bool ok{};

   if (mgr != nullptr) {
      Player* own{static_cast<Player*>(mgr->findContainerByType(typeid(Player)))};
      if (own != nullptr) {
         StoresMgr* sms{own->getStoresManagement()};
         if (sms != nullptr) {
         if (numToLaunch > 1) {
                if (interval == 0.0) {
                    // launch all at once
                    ok = true;
                    for (int i = 0; i < numToLaunch; i++) {
                        sms->releaseOneDecoy();
                    }
                }
                else {
                    // release one, then start counting
                    sms->releaseOneDecoy();
                    tod = own->getWorldModel()->getSimTimeOfDay();
                    startTOD = tod;
                    // decrease our number to launch
                    numToLaunch--;
                    // set our route manager
                    setManager(mgr);
                }
            }
            else {
                ok = true;
                // only one to launch, then just launch one!
                sms->releaseOneDecoy();
            }
         }
      }
   }

   // do we need to launch more at a certain interval?  If so, we aren't complete yet!

   BaseClass::setCompleted(ok);

   return ok;
}

void ActionDecoyRelease::process(const double)
{
    // keep counting until we have our "interval" of seconds
    OnboardComputer* mgr{getManager()};
    if (mgr != nullptr) {
        Player* own{static_cast<Player*>(mgr->findContainerByType(typeid(Player)))};
        if (own != nullptr) {
            tod = own->getWorldModel()->getSimTimeOfDay();
            if (interval < (tod - startTOD)) {
                // we have hit our interval, release another decoy
                StoresMgr* sms = own->getStoresManagement();
                if (sms != nullptr) {
                    sms->releaseOneDecoy();
                    numToLaunch--;
                    if (numToLaunch == 0) {
                        setManager(nullptr);
                        BaseClass::setCompleted(true);
                    }
                    else {
                        // keep going, start counting again!
                        startTOD = tod;
                    }
                }
            }
        }
    }
}

bool ActionDecoyRelease::setSlotNumToLaunch(const base::Integer* x)
{
    bool ok{};
    if (x != nullptr) {
        ok = setNumToLaunch(x->asInt());
    }
    return ok;
}
bool ActionDecoyRelease::setSlotInterval(const base::Number* x)
{
    bool ok{};
    if (x != nullptr) {
        ok = setInterval(x->asDouble());
    }
    return ok;
}


// =============================================================================
// Class: ActionCamouflageType
// =============================================================================

IMPLEMENT_SUBCLASS(ActionCamouflageType, "ActionCamouflageType")
EMPTY_DELETEDATA(ActionCamouflageType)

BEGIN_SLOTTABLE(ActionCamouflageType)
   "camouflageType",    //  1) User defined camouflage type (positive integer or zero for none)
END_SLOTTABLE(ActionCamouflageType)

BEGIN_SLOT_MAP(ActionCamouflageType)
    ON_SLOT( 1, setSlotCamouflageType, base::Integer)
END_SLOT_MAP()

ActionCamouflageType::ActionCamouflageType()
{
   STANDARD_CONSTRUCTOR()
}

void ActionCamouflageType::copyData(const ActionCamouflageType& org, const bool)
{
   BaseClass::copyData(org);

   camouflage = org.camouflage;
}

//------------------------------------------------------------------------------
// Default function: Change our ownship's camouflage type (one-shot)
//------------------------------------------------------------------------------
bool ActionCamouflageType::trigger(OnboardComputer* const mgr)
{
   bool ok{};

   if (mgr != nullptr) {
      Player* own{static_cast<Player*>(mgr->findContainerByType(typeid(Player)))};
      if (own != nullptr) {
         // Set our ownship's camouflage type
         own->setCamouflageType( getCamouflageType() );
         // that's all folks
         BaseClass::setCompleted(true);
         ok = true;
      }
   }

   return ok;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the camouflage type for this player
bool ActionCamouflageType::setCamouflageType(const unsigned int v)
{
   camouflage = v;
   return true;
}

// Sets user defined camouflage type
bool ActionCamouflageType::setSlotCamouflageType(const base::Integer* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int ii{msg->asInt()};
      if (ii >= 0) {
         ok = setCamouflageType( ii );
      }
   }
   return ok;
}

}
}
