
#ifndef __mixr_base_FlowRate_H__
#define __mixr_base_FlowRate_H__

#include "mixr/base/units/Unit.hpp"

namespace mixr {
namespace base {
class Time;
class Volume;

//------------------------------------------------------------------------------
// Class: FlowRate
//
// Description:  Flow Rate as a component of volume and time.
// Factory name: FlowRate
// Slots:
//    volume     <Volume>   ! Volume (default: 1 ft^3)
//    flowTime   <Time>     ! Time (default: 1 second)
//
// set() -- sets our velocity from some other velocity
//     bool FlowRate::set(const double newFlowRate, Volume* newVolume, Time* newTime)
//
// setSlotVolume() -- sets our volume object.
//     bool FlowRate::setSlotVolume(Volume* newVol)
//
// setSlotTime() -- sets our time object.
//     bool FlowRate::setSlotTime(Time* newTime)
//
// set() -- sets our velocity from some other velocity
//     bool FlowRate::set(const double newFlowRate, Volume* newVolume, Time* newTime)
//
// convert() -- converts from one volume/time ratio to another
//     double FlowRate::convert(Volume* newVolume, Time* newTime)
//
//------------------------------------------------------------------------------
class FlowRate : public Unit
{
    DECLARE_SUBCLASS(FlowRate, Unit)

public:
    FlowRate();
    FlowRate(double newFlowRate, Volume*, Time*);

    // dynamic convert routine
    double convert(Volume* newVolume, Time* newTime);
    // get functions
    double getFlowRate()                 { return flowRate; };
    // set functions
    bool set(const double newFlowRate, Volume*, Time*);

private:
    Volume* myVolume{};       // volume
    Time*   myTime{};         // time
    double  flowRate{1.0};    // flow rate

private:
    // slot table helper methods
    bool setSlotVolume(Volume*);
    bool setSlotTime(Time*);
};

}
}

#endif
