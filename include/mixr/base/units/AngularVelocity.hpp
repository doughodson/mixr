
#ifndef __mixr_base_AngularVelocity_H__
#define __mixr_base_AngularVelocity_H__

#include "mixr/base/numeric/Number.hpp"

namespace mixr {
namespace base {
class Angle;
class Time;

//------------------------------------------------------------------------------
// Class AngularVelocity
//
// Description: Angular velocity with internal units of radians/second.
//                Will convert to desired units if needed.
//                    Angles: Radians, Degrees, Semi-Circles
//                    Time: Milliseconds, Minutes, Hours, Days
//                Will convert input units to radians/second from any angle/time unit.
//
// Factory name: AngularVelocity
// Slots:
//    angle  <Angle>  ! angle (default: 1)
//    time   <Time>   ! time (default: 1)
//
//------------------------------------------------------------------------------
//
// Input File Example:
//
//    myAngularVelocity1:
//        ( AngularVelocity
//            angle:    ( Radians 1.5 )
//            time:     ( Seconds 1 )
//        )
//    myAngularVelocity2:
//        ( AngularVelocity
//            angle:    ( Degrees 90 )
//            time:     ( Milliseconds 1 )
//        )
//
// Class Detail:
//
// AngularVelocity();
//  Constructor for input parser:
//
// AngularVelocity(double newAngularVelocityRadiansPerSec);
//  Constructor with input as radians per second:
//
// AngularVelocity(Angle* newAngle, Time* newTime);
//  Constructor with angle in units desired and time in units desired:
//
// double getRadiansPerSecond();
//  Returns in radians/second:
//
// bool setRadiansPerSecond(double newAngularVelocity);
//  Sets in radians/second - returns true if successful:
//
// bool setDegrees(double newAngle);
//  Input angle in (degrees)
//
// bool setRadians(double newAngle);
//  Input angle in (radians)
//
// bool setSeconds(double newTime);
//  Input time in (seconds)
//
// bool setSlotAngle(Angle* angle);
//  Input as type of angle (Radians, Degrees, ...)
//
// bool setSlotTime(Time* time);
//  Input as type of time (Seconds, MilliSeconds, ...)
//
// double convert(Angle* newAngle, Time* newTime);
//  Convert routine - converts angular velocity and outputs in units specified -
// NOTE: This ignores the value of the input objects - the output will be in the
//      units that are used as inputs (Input Degrees and MilliSeconds and convert(~)
//      will output the answer in degrees per millisecond)
//
//--------------------------------------------------------------------
class AngularVelocity : public Number
{
    DECLARE_SUBCLASS(AngularVelocity, Number)

public:

    AngularVelocity();
    // constructor with input as radians per second
    AngularVelocity(const double newAngularVelocityRadiansPerSec);
    // constructor with angle in units desired and time in units desired
    AngularVelocity(const Angle* const newAngle, const Time* const newTime);

    // Returns in radians/second
    double getRadiansPerSecond() const;
    // Convert routine - converts angular velocity and outputs in units specified
    double convert(Angle* newAngle, Time* newTime);

    // Sets Value of Object - Be sure input is in (rad/sec)
    bool setRadiansPerSecond(const double newAngularVelocity);

    bool setDegrees(const double);         // Input angle in (degrees)
    bool setRadians(const double);         // Input angle in (radians)
    bool setSeconds(const double);         // Input time in (seconds)

private:
    double angle {1.0};       // holds our angle (radians)
    double time {1.0};        // holds our time (seconds)

private:
    // slot table helper methods
    bool setSlotAngle(const Angle* const);      // Input as type of angle (Radians, Degrees, ...)
    bool setSlotTime(const Time* const);        // Input as type of time (Seconds, MilliSeconds, ...)
};

}
}

#endif
