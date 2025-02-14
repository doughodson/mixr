
#ifndef __mixr_instruments_DialArcSegment_HPP__
#define __mixr_instruments_DialArcSegment_HPP__

#include "mixr/instruments/dials/AnalogDial.hpp"

namespace mixr {
namespace base { class Boolean; class INumber; }
namespace instruments {

//------------------------------------------------------------------------------
// Class: DialArcSegment
//
// Description: An arc or circle that will be drawn according to the start
// angle and sweep angle set in AnalogDial.
// Input:  UPDATE_INSTRUMENTS (from Instrument)
//------------------------------------------------------------------------------
class DialArcSegment : public AnalogDial
{
    DECLARE_SUBCLASS(DialArcSegment,AnalogDial)

public:
    DialArcSegment();

    // set functions
    virtual bool setIsDynamic(const bool);
    virtual bool setOuterRadius(const double);
    virtual bool setFilled(const bool);

    void drawFunc() override;

    void updateData(const double dt = 0.0) override;

protected:
    bool onUpdateRadius(const base::INumber* const) override;

private:
    bool isDynamic {};          // are we setting sweep angle based on value?
    double outerRadius {0.5};   // just a bit bigger than analog dial
    bool filled {true};

private:
    // slot table helper methods
    bool setSlotIsDynamic(const base::Boolean* const);
    bool setSlotOuterRadius(const base::INumber* const);
    bool setSlotFilled(const base::Boolean* const);
};

}
}

#endif
