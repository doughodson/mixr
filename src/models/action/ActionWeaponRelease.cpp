#include "mixr/models/action/ActionWeaponRelease.hpp"

#include "mixr/models/player/weapon/Bomb.hpp"
#include "mixr/models/player/IPlayer.hpp"
#include "mixr/models/system/OnboardComputer.hpp"
#include "mixr/models/system/Sar.hpp"
#include "mixr/models/system/StoresMgr.hpp"
#include "mixr/models/navigation/Steerpoint.hpp"

#include "mixr/models/WorldModel.hpp"

#include "mixr/base/IComponent.hpp"
#include "mixr/base/Latitude.hpp"
#include "mixr/base/Longitude.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/units/lengths.hpp"
#include "mixr/base/util/nav_utils.hpp"

namespace mixr {
namespace models {

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
    ON_SLOT(3, setSlotTargetElev, base::INumber)
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
      IPlayer* own{static_cast<IPlayer*>(mgr->findContainerByType(typeid(IPlayer)))};
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
            IWeapon* flyout{sms->releaseOneBomb()};
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
bool ActionWeaponRelease::setSlotTargetElev(const base::INumber* newElev)
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

}
}
