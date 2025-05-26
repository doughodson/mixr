
#include "mixr/models/dynamics/IAeroDynamics.hpp"

#include "mixr/base/units/util/angle_utils.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IAeroDynamics, "IAeroDynamics")
EMPTY_SLOTTABLE(IAeroDynamics)
EMPTY_DELETEDATA(IAeroDynamics)

IAeroDynamics::IAeroDynamics()
{
    STANDARD_CONSTRUCTOR()
}

void IAeroDynamics::copyData(const IAeroDynamics& org, const bool)
{
   BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// Data access functions that need conversion
//------------------------------------------------------------------------------

double IAeroDynamics::getAngleOfAttackR() const
{
   return getAngleOfAttack();
}

double IAeroDynamics::getAngleOfAttackD() const
{
   return ( static_cast<double>(base::angle::R2DCC) * getAngleOfAttack() );
}

double IAeroDynamics::getFlightPathR() const
{
   return getFlightPath();
}

double IAeroDynamics::getFlightPathD() const
{
   return ( static_cast<double>(base::angle::R2DCC) * getFlightPath() );
}

double IAeroDynamics::getSideSlipR() const
{
   return getSideSlip();
}

double IAeroDynamics::getSideSlipD() const
{
   return ( static_cast<double>(base::angle::R2DCC) * getSideSlip() );
}

//------------------------------------------------------------------------------
// Get Vehicle data: num engines, thrust, rpm, pla and fuel flow
//------------------------------------------------------------------------------
double IAeroDynamics::getGload() const
{
    return 0.0;
}

double IAeroDynamics::getMach() const
{
    return 0.0;
}

double IAeroDynamics::getAngleOfAttack() const
{
    return 0.0;
}

double IAeroDynamics::getSideSlip() const
{
    return 0.0;
}

double IAeroDynamics::getFlightPath() const
{
    return 0.0;
}

double IAeroDynamics::getCalibratedAirspeed() const
{
    return 0.0;
}

double IAeroDynamics::getAmbientPressureRatio() const
{
   return 1.0;
}

double IAeroDynamics::getWingSweepAngle() const
{
   return 0.0;
}

//------------------------------------------------------------------------------
// setTrimSwitchRollInput(RollTrim) --  Sets the trim switch positions:
//      RollTrim:  -1.0 -> Left;      0.0 -> Hold;  1.0 -> Right
//------------------------------------------------------------------------------
void IAeroDynamics::setTrimSwitchRollInput(const double)
{
}

//------------------------------------------------------------------------------
// setTrimSwitchPitchInput(PitchTrim) --  Sets the trim switch positions:
//      PitchTrim: -1.0 -> NoseDown;  0.0 -> Hold;  1.0 -> NoseUp
//------------------------------------------------------------------------------
void IAeroDynamics::setTrimSwitchPitchInput(const double)
{
}

//------------------------------------------------------------------------------
// setRudderPedalInput(pedal) -- Pedal inputs: normalized
//          pedal:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
void IAeroDynamics::setRudderPedalInput(const double)
{
}

//------------------------------------------------------------------------------
// setFlaps() --
//      Commanded flap position from retracted (0.0) to fully extended (1.0)
//------------------------------------------------------------------------------
void IAeroDynamics::setFlaps(const double)
{
}

//------------------------------------------------------------------------------
// getLandingGearPosition() --   Returns the landing gear position (percent)
//      0-> Fully Retracted;  100.0 -> Fully Extended
//------------------------------------------------------------------------------
double IAeroDynamics::getLandingGearPosition() const
{
    return 0.0;
}

//------------------------------------------------------------------------------
// getWeaponBayDoorPosition() --   Returns the weapon bay door position (percent)
//      0-> closed;  100.0 -> Fully Open
//------------------------------------------------------------------------------
double IAeroDynamics::getWeaponBayDoorPosition() const
{
    return 0.0;
}

//------------------------------------------------------------------------------
// isWeightOnWheels() --   Returns true if there is weight on the wheels.
//------------------------------------------------------------------------------
bool IAeroDynamics::isWeightOnWheels() const
{
    return false;
}

//------------------------------------------------------------------------------
// setGearHandleSwitch() -- Set Gear handle switch position
//                          0 -> Handle up;  1 -> hande down
//------------------------------------------------------------------------------
void IAeroDynamics::setGearHandleSwitch(const double)
{
}

//------------------------------------------------------------------------------
// getSpeedBrakesSwitch() --   Returns the speed brake position (percent)
//      0-> Fully Retracted;  100.0 -> Fully Extended
//------------------------------------------------------------------------------
double IAeroDynamics::getSpeedBrakePosition() const
{
    return 0.0;
}

//------------------------------------------------------------------------------
// setSpeedBrakesSwitch() --   Sets the speed brake switch position:
//      -1.0 -> Retract;  0.0 -> Hold;  1.0 -> Extend
//------------------------------------------------------------------------------
void IAeroDynamics::setSpeedBrakesSwitch(const double)
{
}

//------------------------------------------------------------------------------
// setWeaponBayDoorSwitch() --   Sets the weapon bay door switch position:
//        0.0 -> closed; 1.0 -> open
//------------------------------------------------------------------------------
void IAeroDynamics::setWeaponBayDoorSwitch(const double)
{
}

//------------------------------------------------------------------------------
// Sets the commanded wing sweep angle (radians)
//------------------------------------------------------------------------------
void IAeroDynamics::setCmdWingSweepAngle(const double)
{
}

//------------------------------------------------------------------------------
// Get engine data: num engines, thrust, rpm, pla and fuel flow
//------------------------------------------------------------------------------
int IAeroDynamics::getEngRPM(double* const, const int) const
{
    return 0;
}

int IAeroDynamics::getEngN1(double* const, const int) const
{
    return 0;
}

int IAeroDynamics::getEngPLA(double* const, const int) const
{
    return 0;
}

int IAeroDynamics::getEngFuelFlow(double* const, const int) const
{
    return 0;
}

int IAeroDynamics::getEngOilPressure(double* const, const int) const
{
    return 0;
}

int IAeroDynamics::getEngInletTemp(double* const, const int) const
{
    return 0;
}

int IAeroDynamics::getEngNozzle(double* const, const int) const
{
    return 0;
}

int IAeroDynamics::getEngThrustIdle(double* const, const int) const
{
    return 0;
}

int IAeroDynamics::getEngThrustMil(double* const, const int) const
{
    return 0;
}

int IAeroDynamics::getEngThrustAb(double* const, const int) const
{
    return 0;
}

}
}
