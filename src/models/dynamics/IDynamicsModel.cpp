
#include "mixr/models/dynamics/IDynamicsModel.hpp"
#include <iostream>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IDynamicsModel, "IDynamicsModel")
EMPTY_SLOTTABLE(IDynamicsModel)
EMPTY_DELETEDATA(IDynamicsModel)

IDynamicsModel::IDynamicsModel()
{
    STANDARD_CONSTRUCTOR()
}

void IDynamicsModel::copyData(const IDynamicsModel& org, const bool)
{
   BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// dynamics() -- one pass update of this dynamics model
//               (called from Player::dynamics)
//------------------------------------------------------------------------------
void IDynamicsModel::dynamics(const double)
{
}

//------------------------------------------------------------------------------
// atReleaseInit() -- init the model at the transition from PRE_RELEASE to
// ACTIVE mode.  Default is to call reset().  Used by Weapon players.
//------------------------------------------------------------------------------
void IDynamicsModel::atReleaseInit()
{
   reset();
}

//------------------------------------------------------------------------------
// Access functions
//------------------------------------------------------------------------------
double IDynamicsModel::getFuelWt() const
{
    return 0.0;
}

double IDynamicsModel::getFuelWtMax() const
{
    return 0.0;
}

double IDynamicsModel::getGrossWeight() const
{
    return 0.0;
}

int IDynamicsModel::getNumberOfEngines() const
{
    return 0;
}

int IDynamicsModel::getEngThrust(double* const, const int) const
{
    return 0;
}



//------------------------------------------------------------------------------
// Autopilot controls
//------------------------------------------------------------------------------
bool IDynamicsModel::isHeadingHoldOn() const
{
   return false;
}

double IDynamicsModel::getCommandedHeadingD() const
{
   return 0;
}

// setHeadingHoldOn() --   Enable/Disable heading hold
bool IDynamicsModel::setHeadingHoldOn(const bool)
{
   return false;
}

// setCommandedHeadingD() --   Sets commanded heading (true: degs)
bool IDynamicsModel::setCommandedHeadingD(const double, const double, const double)
{
   return false;
}

bool IDynamicsModel::isVelocityHoldOn() const
{
   return false;
}

double IDynamicsModel::getCommandedVelocityKts() const
{
   return 0;
}

// setVelocityHoldOn() --   Enable/Disable velocity hold
bool IDynamicsModel::setVelocityHoldOn(const bool)
{
   return false;
}

// setCommandedVelocityKts() --   Sets commanded velocity (kts)
bool IDynamicsModel::setCommandedVelocityKts(const double, const double)
{
   return false;
}

bool IDynamicsModel::isAltitudeHoldOn() const
{
   return false;
}

double IDynamicsModel::getCommandedAltitude() const
{
   return 0;
}

// setAltitudeHoldOn() --   Enable/Disable altitude hold
bool IDynamicsModel::setAltitudeHoldOn(const bool)
{
   return false;
}

// setCommandedAltitude() --   Sets commanded altitude (meters)
bool IDynamicsModel::setCommandedAltitude(const double, const double, const double)
{
   return false;
}

//------------------------------------------------------------------------------
// setControlStickRollInput(Roll) --  Control inputs: normalized
//   roll:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
void IDynamicsModel::setControlStickRollInput(const double)
{
}

//------------------------------------------------------------------------------
// setControlStickPitchInput(Pitch) --  Control inputs: normalized
//  pitch:  -1.0 -> max forward (nose down); 0.0 -> center;  1.0 -> max back (nose up)
//------------------------------------------------------------------------------
void IDynamicsModel::setControlStickPitchInput(const double)
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
int IDynamicsModel::setThrottles(const double* const, const int)
{
    return 0;
}

//------------------------------------------------------------------------------
// setBrakes() -- Sets brake positions (left & right)
//               No brake force  -> 0.0
//               Max brake force -> 1.0
//------------------------------------------------------------------------------
void IDynamicsModel::setBrakes(const double, const double)
{
}

//------------------------------------------------------------------------------
// Sets the fuel weight
//------------------------------------------------------------------------------
bool IDynamicsModel::setFuelWt(const double)
{
   return false;
}

}
}
