
#include "mixr/models/dynamics/IDynamicsAero.hpp"

#include "mixr/base/units/util/angle_utils.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IDynamicsAero, "IDynamicsAero")
EMPTY_SLOTTABLE(IDynamicsAero)
EMPTY_DELETEDATA(IDynamicsAero)

IDynamicsAero::IDynamicsAero()
{
    STANDARD_CONSTRUCTOR()
}

void IDynamicsAero::copyData(const IDynamicsAero& org, const bool)
{
   BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// Data access functions that need conversion
//------------------------------------------------------------------------------

double IDynamicsAero::getAngleOfAttackR() const
{
   return getAngleOfAttack();
}

double IDynamicsAero::getAngleOfAttackD() const
{
   return ( static_cast<double>(base::angle::R2DCC) * getAngleOfAttack() );
}

double IDynamicsAero::getFlightPathR() const
{
   return getFlightPath();
}

double IDynamicsAero::getFlightPathD() const
{
   return ( static_cast<double>(base::angle::R2DCC) * getFlightPath() );
}

double IDynamicsAero::getSideSlipR() const
{
   return getSideSlip();
}

double IDynamicsAero::getSideSlipD() const
{
   return ( static_cast<double>(base::angle::R2DCC) * getSideSlip() );
}

//------------------------------------------------------------------------------
// Get Vehicle data: num engines, thrust, rpm, pla and fuel flow
//------------------------------------------------------------------------------
double IDynamicsAero::getGload() const
{
    return 0.0;
}

double IDynamicsAero::getMach() const
{
    return 0.0;
}

double IDynamicsAero::getAngleOfAttack() const
{
    return 0.0;
}

double IDynamicsAero::getSideSlip() const
{
    return 0.0;
}

double IDynamicsAero::getFlightPath() const
{
    return 0.0;
}

double IDynamicsAero::getCalibratedAirspeed() const
{
    return 0.0;
}

double IDynamicsAero::getAmbientPressureRatio() const
{
   return 1.0;
}

double IDynamicsAero::getWingSweepAngle() const
{
   return 0.0;
}

//------------------------------------------------------------------------------
// setTrimSwitchRollInput(RollTrim) --  Sets the trim switch positions:
//      RollTrim:  -1.0 -> Left;      0.0 -> Hold;  1.0 -> Right
//------------------------------------------------------------------------------
void IDynamicsAero::setTrimSwitchRollInput(const double)
{
}

//------------------------------------------------------------------------------
// setTrimSwitchPitchInput(PitchTrim) --  Sets the trim switch positions:
//      PitchTrim: -1.0 -> NoseDown;  0.0 -> Hold;  1.0 -> NoseUp
//------------------------------------------------------------------------------
void IDynamicsAero::setTrimSwitchPitchInput(const double)
{
}

//------------------------------------------------------------------------------
// setRudderPedalInput(pedal) -- Pedal inputs: normalized
//          pedal:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
void IDynamicsAero::setRudderPedalInput(const double)
{
}

//------------------------------------------------------------------------------
// setFlaps() --
//      Commanded flap position from retracted (0.0) to fully extended (1.0)
//------------------------------------------------------------------------------
void IDynamicsAero::setFlaps(const double)
{
}

//------------------------------------------------------------------------------
// getLandingGearPosition() --   Returns the landing gear position (percent)
//      0-> Fully Retracted;  100.0 -> Fully Extended
//------------------------------------------------------------------------------
double IDynamicsAero::getLandingGearPosition() const
{
    return 0.0;
}

//------------------------------------------------------------------------------
// getWeaponBayDoorPosition() --   Returns the weapon bay door position (percent)
//      0-> closed;  100.0 -> Fully Open
//------------------------------------------------------------------------------
double IDynamicsAero::getWeaponBayDoorPosition() const
{
    return 0.0;
}

//------------------------------------------------------------------------------
// isWeightOnWheels() --   Returns true if there is weight on the wheels.
//------------------------------------------------------------------------------
bool IDynamicsAero::isWeightOnWheels() const
{
    return false;
}

//------------------------------------------------------------------------------
// setGearHandleSwitch() -- Set Gear handle switch position
//                          0 -> Handle up;  1 -> hande down
//------------------------------------------------------------------------------
void IDynamicsAero::setGearHandleSwitch(const double)
{
}

//------------------------------------------------------------------------------
// getSpeedBrakesSwitch() --   Returns the speed brake position (percent)
//      0-> Fully Retracted;  100.0 -> Fully Extended
//------------------------------------------------------------------------------
double IDynamicsAero::getSpeedBrakePosition() const
{
    return 0.0;
}

//------------------------------------------------------------------------------
// setSpeedBrakesSwitch() --   Sets the speed brake switch position:
//      -1.0 -> Retract;  0.0 -> Hold;  1.0 -> Extend
//------------------------------------------------------------------------------
void IDynamicsAero::setSpeedBrakesSwitch(const double)
{
}

//------------------------------------------------------------------------------
// setWeaponBayDoorSwitch() --   Sets the weapon bay door switch position:
//        0.0 -> closed; 1.0 -> open
//------------------------------------------------------------------------------
void IDynamicsAero::setWeaponBayDoorSwitch(const double)
{
}

//------------------------------------------------------------------------------
// Sets the commanded wing sweep angle (radians)
//------------------------------------------------------------------------------
void IDynamicsAero::setCmdWingSweepAngle(const double)
{
}

//------------------------------------------------------------------------------
// Get engine data: num engines, thrust, rpm, pla and fuel flow
//------------------------------------------------------------------------------
int IDynamicsAero::getEngRPM(double* const, const int) const
{
    return 0;
}

int IDynamicsAero::getEngN1(double* const, const int) const
{
    return 0;
}

int IDynamicsAero::getEngPLA(double* const, const int) const
{
    return 0;
}

int IDynamicsAero::getEngFuelFlow(double* const, const int) const
{
    return 0;
}

int IDynamicsAero::getEngOilPressure(double* const, const int) const
{
    return 0;
}

int IDynamicsAero::getEngInletTemp(double* const, const int) const
{
    return 0;
}

int IDynamicsAero::getEngNozzle(double* const, const int) const
{
    return 0;
}

int IDynamicsAero::getEngThrustIdle(double* const, const int) const
{
    return 0;
}

int IDynamicsAero::getEngThrustMil(double* const, const int) const
{
    return 0;
}

int IDynamicsAero::getEngThrustAb(double* const, const int) const
{
    return 0;
}

}
}
