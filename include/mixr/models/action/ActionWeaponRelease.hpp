#ifndef __mixr_models_ActionWeaponRelease_HPP__
#define __mixr_models_ActionWeaponRelease_HPP__

#include "mixr/models/action/IAction.hpp"
#include "mixr/base/safe_ptr.hpp"

namespace mixr {
namespace base { class Integer; class Latitude; class ILength; class Longitude; class INumber; }
namespace models {
class OnboardComputer;

//------------------------------------------------------------------------------
// Class: ActionWeaponRelease
// Description: Concrete - Launches a weapon
//
// Factory name: ActionWeaponRelease
// Slots:
//   targetLatitude   <Latitude>  ! Target's latitude (default: 0)
//   targetLongitude  <Longitude> ! Target's longitude (default: 0)
//   targetElevation  <INumber>   ! Target's elevation (feet) (default: 0)
//   station          <Integer>   ! Station number to use (default: 0)
//
//------------------------------------------------------------------------------
class ActionWeaponRelease final: public IAction
{
    DECLARE_SUBCLASS(ActionWeaponRelease, IAction)

public:
   ActionWeaponRelease();

   double getTargetLatitude() const       { return targetLatitude; }    // Target latitude in degrees
   double getTargetLongitude() const      { return targetLongitude; }   // Target longitude in degrees
   double getTargetElevation() const      { return targetElevation; }   // Target elevation in meters
   unsigned int getStation() const        { return station; }           // Weapon station number

   // Sets the target location (latitude & longitude in radians, elevation in meters)
   virtual bool setTargetLocation(const double tgtLat, const double tgtLon, const double tgtElev);

   // Set planned station number
   virtual bool setStation(const unsigned int num);

   bool trigger(OnboardComputer* const mgr) override;

private:
   double targetLatitude {};      // latitude (deg)
   double targetLongitude {};     // longitude (deg)
   double targetElevation {};     // elevation (meters)
   unsigned int station {};       // station number

private:
   bool setSlotTargetLat(const base::Latitude*);
   bool setSlotTargetLon(const base::Longitude*);
   bool setSlotTargetElev(const base::INumber*);
   bool setSlotStationNum(const base::Integer*);
};

}
}

#endif

