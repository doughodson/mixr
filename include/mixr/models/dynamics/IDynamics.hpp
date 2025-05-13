
#ifndef __mixr_models_common_dynamics_IDynamics_HPP__
#define __mixr_models_common_dynamics_IDynamics_HPP__

#include "mixr/base/IComponent.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: IDynamics
//
// Description: Interface class for all dynamics models
//
// Note --
//
//    1) updateTC() and updateData() are called by Component::upateTC() and
//    Component::updateData() as part of the player's component list.
//
//    2) To be properly synchronized within a frame, the root of our dynamics
//       model should be in our dynamics() function, which will called from
//       the Player class dynamics() function.
//
//    3) atReleaseInit() is called by Weapon::updateTC() to switch from
//       PRE_RELEASE mode to ACTIVE mode.  Default is to call reset().
//       Not used by air vehicles.
//
//    4) This class is one of the "top level" systems attached to a Player
//       class (see Player.hpp).
//
// Factory name: IDynamics
//
//------------------------------------------------------------------------------
class IDynamics : public base::IComponent
{
    DECLARE_SUBCLASS(IDynamics, base::IComponent)

public:
    IDynamics();

    virtual void atReleaseInit();
    virtual void dynamics(const double dt);

    virtual bool isHeadingHoldOn() const;
    virtual double getCommandedHeadingD() const;
    virtual bool setHeadingHoldOn(const bool);

    // commanded heading inputs:  true heading in degrees (h),
    //                            degrees per second rate of heading change (hDps)
    //                            and maximum bank angle (maxBank)
    virtual bool setCommandedHeadingD(const double h, const double hDps = 0, const double maxBank = 0);

    virtual bool isVelocityHoldOn() const;
    virtual double getCommandedVelocityKts() const;
    virtual bool setVelocityHoldOn(const bool);

    // commanded velocity inputs: commanded velocity in knots (v),
    //                            acceleration limiter (vNps)
    // acceleration limiter is determines how fast the model gets to the actual commanded velocity
    virtual bool setCommandedVelocityKts(const double v, const double vNps = 0);

    virtual bool isAltitudeHoldOn() const;
    virtual double getCommandedAltitude() const;
    virtual bool setAltitudeHoldOn(const bool);

    // commanded altitude  inputs: commanded altitude in meters (a),
    //                             rate of change (in meters per second) associated with moving to new altitude (aMps)
    virtual bool setCommandedAltitude(const double a, const double aMps = 0, const double maxPitch = 0);

    virtual double getFuelWt() const;                   // lbs
    virtual double getFuelWtMax() const;                // lbs
    virtual double getGrossWeight() const;              // wt: lbs

    virtual int getNumberOfEngines() const;
    virtual int getEngThrust(double* const fn, const int max) const;          // Thrust: lbs

    // Control Stick Input Positions (not the flight surfaces!)
    virtual void setControlStickRollInput(const double);
    virtual void setControlStickPitchInput(const double);
    void setControlStick(const double roll, const double pitch) {
        setControlStickRollInput(roll);
        setControlStickPitchInput(pitch);
    }

    // ---
    // Set throttle positions; returns the actual number of throttle positions
    //    'positions' -> Array of throttle positions (for each throttle)
    //              < 0.0  -> Cutoff
    //              0.0    -> Idle
    //              1.0    -> MIL
    //              2.0    -> A/B
    //    'num '-> number of throttle positions to get/set
    // ---
    virtual int setThrottles(const double* const positions, const int num);

    // ---
    // Sets brake positions (left & right)
    //         No brake force  -> 0.0
    //         Max brake force -> 1.0
    // ---
    virtual void setBrakes(const double left, const double right);

    // Sets the fuel weight (lbs)
    virtual bool setFuelWt(const double lbs);
};

}
}

#endif
