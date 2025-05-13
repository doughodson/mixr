
#include "mixr/models/dynamics/IDynamics.hpp"
#include <iostream>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IDynamics, "IDynamics")
EMPTY_SLOTTABLE(IDynamics)
EMPTY_DELETEDATA(IDynamics)

IDynamics::IDynamics()
{
    STANDARD_CONSTRUCTOR()
}

void IDynamics::copyData(const IDynamics& org, const bool)
{
   BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// dynamics() -- one pass update of this dynamics model
//               (called from Player::dynamics)
//------------------------------------------------------------------------------
void IDynamics::dynamics(const double)
{
}

//------------------------------------------------------------------------------
// atReleaseInit() -- init the model at the transition from PRE_RELEASE to
// ACTIVE mode.  Default is to call reset().  Used by Weapon players.
//------------------------------------------------------------------------------
void IDynamics::atReleaseInit()
{
   reset();
}

//------------------------------------------------------------------------------
// Access functions
//------------------------------------------------------------------------------
double IDynamics::getFuelWt() const
{
    return 0.0;
}

double IDynamics::getFuelWtMax() const
{
    return 0.0;
}

double IDynamics::getGrossWeight() const
{
    return 0.0;
}

int IDynamics::getNumberOfEngines() const
{
    return 0;
}

int IDynamics::getEngThrust(double* const, const int) const
{
    return 0;
}



//------------------------------------------------------------------------------
// Autopilot controls
//------------------------------------------------------------------------------
bool IDynamics::isHeadingHoldOn() const
{
   return false;
}

double IDynamics::getCommandedHeadingD() const
{
   return 0;
}

// setHeadingHoldOn() --   Enable/Disable heading hold
bool IDynamics::setHeadingHoldOn(const bool)
{
   return false;
}

// setCommandedHeadingD() --   Sets commanded heading (true: degs)
bool IDynamics::setCommandedHeadingD(const double, const double, const double)
{
   return false;
}

bool IDynamics::isVelocityHoldOn() const
{
   return false;
}

double IDynamics::getCommandedVelocityKts() const
{
   return 0;
}

// setVelocityHoldOn() --   Enable/Disable velocity hold
bool IDynamics::setVelocityHoldOn(const bool)
{
   return false;
}

// setCommandedVelocityKts() --   Sets commanded velocity (kts)
bool IDynamics::setCommandedVelocityKts(const double, const double)
{
   return false;
}

bool IDynamics::isAltitudeHoldOn() const
{
   return false;
}

double IDynamics::getCommandedAltitude() const
{
   return 0;
}

// setAltitudeHoldOn() --   Enable/Disable altitude hold
bool IDynamics::setAltitudeHoldOn(const bool)
{
   return false;
}

// setCommandedAltitude() --   Sets commanded altitude (meters)
bool IDynamics::setCommandedAltitude(const double, const double, const double)
{
   return false;
}

//------------------------------------------------------------------------------
// setControlStickRollInput(Roll) --  Control inputs: normalized
//   roll:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
void IDynamics::setControlStickRollInput(const double)
{
}

//------------------------------------------------------------------------------
// setControlStickPitchInput(Pitch) --  Control inputs: normalized
//  pitch:  -1.0 -> max forward (nose down); 0.0 -> center;  1.0 -> max back (nose up)
//------------------------------------------------------------------------------
void IDynamics::setControlStickPitchInput(const double)
{
}

//------------------------------------------------------------------------------
// int setThrottles(positions,num) -- Set throttle positions
//
//    positions -> Array of throttle positions
//      (for each throttle)
//              < 0.0       -> Cutoff
//              0.0         -> Idle
//              1.0         -> MIL
//              2.0         -> A/B
//    num -> number of throttle positions to get/set
//    returns the actual number of throttle positions
//------------------------------------------------------------------------------
int IDynamics::setThrottles(const double* const, const int)
{
    return 0;
}

//------------------------------------------------------------------------------
// setBrakes() -- Sets brake positions (left & right)
//               No brake force  -> 0.0
//               Max brake force -> 1.0
//------------------------------------------------------------------------------
void IDynamics::setBrakes(const double, const double)
{
}

//------------------------------------------------------------------------------
// Sets the fuel weight
//------------------------------------------------------------------------------
bool IDynamics::setFuelWt(const double)
{
   return false;
}

}
}
