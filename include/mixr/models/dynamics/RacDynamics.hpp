
#ifndef __mixr_models_common_RacDynamics_HPP__
#define __mixr_models_common_RacDynamics_HPP__

#include "mixr/models/dynamics/IDynamicsAero.hpp"

namespace mixr {
namespace base { class IAngle; class ILength; class INumber; }
namespace models {

//------------------------------------------------------------------------------
// Class: RacDynamics
//
// Description: Robot Aircraft (RAC) Air Vehicle Dynamics Model
//                -- Very simple dynamics model
//
// Factory name: RacDynamics
// Slots:
//    minSpeed       <INumber>   ! Minimum Velocity        (kts)
//    speedMaxG      <INumber>   ! Velocity we reach max G (kts)
//    maxg           <INumber>   ! Max G's (at "speedMaxG" or above)
//    maxAccel       <INumber>   ! Maximum Acceleration    (m/s/s)
//    cmdAltitude    <Length>    ! Command Altitude
//    cmdHeading     <Angle>     ! Command Heading
//    cmdSpeed       <INumber>   ! Command speed           (kts)
//------------------------------------------------------------------------------
class RacDynamics final: public IDynamicsAero
{
    DECLARE_SUBCLASS(RacDynamics, IDynamicsAero)

public:
    RacDynamics();

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
   bool setSlotMinSpeed(const base::INumber* const);
   bool setSlotSpeedMaxG(const base::INumber* const);
   bool setSlotMaxG(const base::INumber* const);
   bool setSlotMaxAccel(const base::INumber* const);
   bool setSlotCmdAltitude(const base::ILength* const);
   bool setSlotCmdHeading(const base::IAngle* const);
   bool setSlotCmdVelocity(const base::INumber* const);
};

}
}

#endif
