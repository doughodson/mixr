
#ifndef __mixr_instruments_AnalogDial_H__
#define __mixr_instruments_AnalogDial_H__

#include "mixr/instruments/Instrument.hpp"

namespace mixr {
namespace instruments {

//------------------------------------------------------------------------------
// Class: AnalogDial
//
// Description:  Used as a parent class for other dials, it will draw a background
// that can be visible or not, depending on a flag.  It can also rotate according
// to a scale, or it can be fixed.  This is a generic intelligent background
// that will also pass along instrument values down to its components.
//
// Inputs:
//      UPDATE_INSTRUMENTS -> (from instrument), sets our rotation angle
//      UPDATE_VALUE -> setRadius (inches)
//------------------------------------------------------------------------------
class AnalogDial : public Instrument
{
    DECLARE_SUBCLASS(AnalogDial, Instrument)

public:
    AnalogDial();

    virtual bool setOriginAngle(const double);
    virtual bool setSweepAngle(const double);
    virtual bool setRadius(const double);
    virtual bool setMobile(const bool);
    virtual bool setSlices(const int);

    // here are the get functions
    double getStartAngle() const    { return originAngle; }
    double getSweepAngle() const    { return sweepAngle; }
    bool getMobile() const          { return isMobile; }
    double getRadius() const        { return radius; }
    int getSlices() const           { return slices; }

    void drawFunc() override;

    bool event(const int event, base::Object* const obj = nullptr) override;

protected:
    // event function
    virtual bool onUpdateRadius(const base::Number* const);

private:
    double originAngle {};     // angle we start drawing ticks from (degrees, default is 0)
    double positionAngle {};   // our position (if we are being rotated)
    double sweepAngle {360};   // how far around the circle we sweep
    double radius {};          // radius of our background
    bool isMobile {};          // are we moving around on the dial, or just sending the value down (to our components)
    int slices {1000};         // number of slices to use while drawing

private:
    // slot table helper methods
    bool setSlotOriginAngle(const base::Number* const);
    bool setSlotMobile(const base::Number* const);
    bool setSlotSweepAngle(const base::Number* const);
    bool setSlotRadius(const base::Number* const);
    bool setSlotSlices(const base::Number* const);
};

}
}

#endif
