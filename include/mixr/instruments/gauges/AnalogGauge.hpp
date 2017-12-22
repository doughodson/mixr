
#ifndef __mixr_instruments_AnalogGuage_H__
#define __mixr_instruments_AnalogGuage_H__

#include "mixr/instruments/Instrument.hpp"

namespace mixr {
namespace instruments {

//------------------------------------------------------------------------------
// Class: AnalogGauge
//
// Description: This will create a gauge that will
// draw vertically or horizontally depending on a flag.
//
// Inputs:
//      UPDATE_INSTRUMENTS (from instrument)
//
// Public member functions:
//      setLeftBoundary() - where to draw the left side of the gauge
//      setRightBoundary() - where to draw the right side of the gauge
//      setIsOutlined() - if true, the gauge isn't filled
//      setIsVertical() - if true, the gauge slides up and down
//------------------------------------------------------------------------------
class AnalogGauge : public Instrument
{
    DECLARE_SUBCLASS(AnalogGauge, Instrument)

public:
    AnalogGauge();

    virtual bool setLeftBoundary(const double);
    virtual bool setRightBoundary(const double);
    virtual bool setIsOutlined(const bool);
    virtual bool setIsVertical(const bool);
    virtual bool setDrawMe(const bool);

    double getLeftBoundary() const  { return leftBoundary; }
    double getRightBoundary() const { return rightBoundary; }
    bool isOutlined() const         { return outline; }
    bool isVertical() const         { return vertical; }
    bool isDrawing() const          { return drawMe; }

    void drawFunc() override;

private:
    double gaugePos {};           // direction, in inches, that we want to scale our gauge
    double leftBoundary {-1.0};   // our left boundary (in inches)
    double rightBoundary {1.0};   // our right boundary (in inches)
    bool outline {};              // are we an outline or filled?
    bool vertical {true};         // are we sliding vertically or horizontally? (True = vertical)
    bool drawMe {true};           // this is for derived classes that don't want us to draw

private:
    // slot table helper methods
    bool setSlotLeftBoundary(const base::Number* const);
    bool setSlotRightBoundary(const base::Number* const);
    bool setSlotIsOutlined(const base::Number* const);
    bool setSlotIsVertical(const base::Number* const);
};

}
}

#endif
