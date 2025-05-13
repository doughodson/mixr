
#include "mixr/models/dynamics/IAerodynamics.hpp"

#include "mixr/base/units/util/angle_utils.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IAerodynamics, "IAerodynamics")
EMPTY_SLOTTABLE(IAerodynamics)
EMPTY_DELETEDATA(IAerodynamics)

IAerodynamics::IAerodynamics()
{
    STANDARD_CONSTRUCTOR()
}

void IAerodynamics::copyData(const IAerodynamics& org, const bool)
{
   BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// Data access functions that need conversion
//------------------------------------------------------------------------------

double IAerodynamics::getAngleOfAttackR() const
{
   return getAngleOfAttack();
}

double IAerodynamics::getAngleOfAttackD() const
{
   return ( static_cast<double>(base::angle::R2DCC) * getAngleOfAttack() );
}

double IAerodynamics::getFlightPathR() const
{
   return getFlightPath();
}

double IAerodynamics::getFlightPathD() const
{
   return ( static_cast<double>(base::angle::R2DCC) * getFlightPath() );
}

double IAerodynamics::getSideSlipR() const
{
   return getSideSlip();
}

double IAerodynamics::getSideSlipD() const
{
   return ( static_cast<double>(base::angle::R2DCC) * getSideSlip() );
}

//------------------------------------------------------------------------------
// Get Vehicle data: num engines, thrust, rpm, pla and fuel flow
//------------------------------------------------------------------------------
double IAerodynamics::getGload() const
{
    return 0.0;
}

double IAerodynamics::getMach() const
{
    return 0.0;
}

double IAerodynamics::getAngleOfAttack() const
{
    return 0.0;
}

double IAerodynamics::getSideSlip() const
{
    return 0.0;
}

double IAerodynamics::getFlightPath() const
{
    return 0.0;
}

double IAerodynamics::getCalibratedAirspeed() const
{
    return 0.0;
}

double IAerodynamics::getAmbientPressureRatio() const
{
   return 1.0;
}

double IAerodynamics::getWingSweepAngle() const
{
   return 0.0;
}

//------------------------------------------------------------------------------
// setTrimSwitchRollInput(RollTrim) --  Sets the trim switch positions:
//      RollTrim:  -1.0 -> Left;      0.0 -> Hold;  1.0 -> Right
//------------------------------------------------------------------------------
void IAerodynamics::setTrimSwitchRollInput(const double)
{
}

//------------------------------------------------------------------------------
// setTrimSwitchPitchInput(PitchTrim) --  Sets the trim switch positions:
//      PitchTrim: -1.0 -> NoseDown;  0.0 -> Hold;  1.0 -> NoseUp
//------------------------------------------------------------------------------
void IAerodynamics::setTrimSwitchPitchInput(const double)
{
}

//------------------------------------------------------------------------------
// setRudderPedalInput(pedal) -- Pedal inputs: normalized
//          pedal:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
void IAerodynamics::setRudderPedalInput(const double)
{
}

//------------------------------------------------------------------------------
// setFlaps() --
//      Commanded flap position from retracted (0.0) to fully extended (1.0)
//------------------------------------------------------------------------------
void IAerodynamics::setFlaps(const double)
{
}

//------------------------------------------------------------------------------
// getLandingGearPosition() --   Returns the landing gear position (percent)
//      0-> Fully Retracted;  100.0 -> Fully Extended
//------------------------------------------------------------------------------
double IAerodynamics::getLandingGearPosition() const
{
    return 0.0;
}

//------------------------------------------------------------------------------
// getWeaponBayDoorPosition() --   Returns the weapon bay door position (percent)
//      0-> closed;  100.0 -> Fully Open
//------------------------------------------------------------------------------
double IAerodynamics::getWeaponBayDoorPosition() const
{
    return 0.0;
}

//------------------------------------------------------------------------------
// isWeightOnWheels() --   Returns true if there is weight on the wheels.
//------------------------------------------------------------------------------
bool IAerodynamics::isWeightOnWheels() const
{
    return false;
}

//------------------------------------------------------------------------------
// setGearHandleSwitch() -- Set Gear handle switch position
//                          0 -> Handle up;  1 -> hande down
//------------------------------------------------------------------------------
void IAerodynamics::setGearHandleSwitch(const double)
{
}

//------------------------------------------------------------------------------
// getSpeedBrakesSwitch() --   Returns the speed brake position (percent)
//      0-> Fully Retracted;  100.0 -> Fully Extended
//------------------------------------------------------------------------------
double IAerodynamics::getSpeedBrakePosition() const
{
    return 0.0;
}

//------------------------------------------------------------------------------
// setSpeedBrakesSwitch() --   Sets the speed brake switch position:
//      -1.0 -> Retract;  0.0 -> Hold;  1.0 -> Extend
//------------------------------------------------------------------------------
void IAerodynamics::setSpeedBrakesSwitch(const double)
{
}

//------------------------------------------------------------------------------
// setWeaponBayDoorSwitch() --   Sets the weapon bay door switch position:
//        0.0 -> closed; 1.0 -> open
//------------------------------------------------------------------------------
void IAerodynamics::setWeaponBayDoorSwitch(const double)
{
}

//------------------------------------------------------------------------------
// Sets the commanded wing sweep angle (radians)
//------------------------------------------------------------------------------
void IAerodynamics::setCmdWingSweepAngle(const double)
{
}

//------------------------------------------------------------------------------
// Get engine data: num engines, thrust, rpm, pla and fuel flow
//------------------------------------------------------------------------------
int IAerodynamics::getEngRPM(double* const, const int) const
{
    return 0;
}

int IAerodynamics::getEngN1(double* const, const int) const
{
    return 0;
}

int IAerodynamics::getEngPLA(double* const, const int) const
{
    return 0;
}

int IAerodynamics::getEngFuelFlow(double* const, const int) const
{
    return 0;
}

int IAerodynamics::getEngOilPressure(double* const, const int) const
{
    return 0;
}

int IAerodynamics::getEngInletTemp(double* const, const int) const
{
    return 0;
}

int IAerodynamics::getEngNozzle(double* const, const int) const
{
    return 0;
}

int IAerodynamics::getEngThrustIdle(double* const, const int) const
{
    return 0;
}

int IAerodynamics::getEngThrustMil(double* const, const int) const
{
    return 0;
}

int IAerodynamics::getEngThrustAb(double* const, const int) const
{
    return 0;
}

}
}
