
#ifndef __mixr_models_RacModel_H__
#define __mixr_models_RacModel_H__

#include "mixr/models/dynamics/AerodynamicsModel.hpp"

namespace mixr {
namespace base { class Angle; class Distance; class Number; }
namespace models {

//------------------------------------------------------------------------------
// Class: RacModel
//
// Description: Robot Aircraft (RAC) Air Vehicle Dynamics Model
//                -- Very simple dynamics model
//
// Factory name: RacModel
// Slots:
//    minSpeed       <Number>    ! Minimum Velocity        (kts)
//    speedMaxG      <Number>    ! Velocity we reach max G (kts)
//    maxg           <Number>    ! Max G's (at "speedMaxG" or above)
//    maxAccel       <Number>    ! Maximum Acceleration    (m/s/s)
//    cmdAltitude    <Distance>  ! Command Altitude
//    cmdHeading     <Angle>     ! Command Heading
//    cmdSpeed       <Number>    ! Command speed           (kts)
//------------------------------------------------------------------------------
class RacModel final: public AerodynamicsModel
{
    DECLARE_SUBCLASS(RacModel, AerodynamicsModel)

public:
    RacModel();

    double getGload() const final;                   // G's (+up, one at level)
    double getMach() const final;                    // mach number
    double getAngleOfAttack() const final;           // radians
    double getSideSlip() const final;                // radians
    double getFlightPath() const final;              // radians
    double getCalibratedAirspeed() const final;      // Kts

    bool isHeadingHoldOn() const final;
    double getCommandedHeadingD() const final;
    bool setHeadingHoldOn(const bool b) final;
    bool setCommandedHeadingD(const double h, const double hDps = 0, const double maxBank = 0) final;
    bool isVelocityHoldOn() const final;
    double getCommandedVelocityKts() const final;
    bool setVelocityHoldOn(const bool b) final;
    bool setCommandedVelocityKts(const double v, const double vNps = 0) final;
    bool isAltitudeHoldOn() const final;
    double getCommandedAltitude() const final;
    bool setAltitudeHoldOn(const bool) final;
    bool setCommandedAltitude(const double a, const double aMps = 0, const double maxPitch = 0) final;
    void dynamics(const double dt) final;            // One pass model update; called from Player::dynamics()

    void reset() final;

private:
    void updateRAC(const double dt);

    double vpMin{};               // Minimum Velocity              (m/s)
    double vpMaxG{250.0};         // Velocity for Max G's          (g's)
    double gMax{4.0};             // Max G's                       (g's)
    double maxAccel{10.0};        // Max longitudinal acceleration (m/s/s)
    double cmdAltitude{-9999.0};  // Commanded Altitude            (meters)
    double cmdHeading{-9999.0};   // Commanded Heading             (degs)
    double cmdVelocity{-9999.0};  // Commanded speed               (kts)

private:
   // slot table helper methods
   bool setSlotMinSpeed(const base::Number* const);
   bool setSlotSpeedMaxG(const base::Number* const);
   bool setSlotMaxG(const base::Number* const);
   bool setSlotMaxAccel(const base::Number* const);
   bool setSlotCmdAltitude(const base::Distance* const);
   bool setSlotCmdHeading(const base::Angle* const);
   bool setSlotCmdVelocity(const base::Number* const);
};

}
}

#endif
