
#ifndef __mixr_instruments_TickMarks_HPP__
#define __mixr_instruments_TickMarks_HPP__

#include "mixr/instruments/gauges/AnalogGauge.hpp"

namespace mixr {
namespace base { class Boolean; class Integer; class INumber; }
namespace instruments {

//------------------------------------------------------------------------------
// Class: TickMarks
//
// Description: Uses tick marks in a linear fashion, either horizontally or
// vertically, and draws them accordingly.  This is very similar to dial
// tick marks, only it doesn't rotate and doesn't draw around the circle.
//------------------------------------------------------------------------------
class TickMarks : public AnalogGauge
{
    DECLARE_SUBCLASS(TickMarks,AnalogGauge)

public:
    TickMarks();

    virtual bool setTickMarkLength(const double);
    virtual bool setQuantity(const int);
    virtual bool setGaugeLength(const double);
    virtual bool setFlip(const bool);
    virtual bool setTickGraphic(const graphics::Graphic* const);

    double getTickMarkLength() const { return lengthTM; }
    int  getQuantity() const         { return quantity; }

    void drawFunc() override;

private:
    double lengthTM{1.0};            // tick mark length (if not a graphic)
    int    quantity{1};              // how many tick marks will we have?
    double gaugeLength{};            // length we are spanning our tick marks over
    bool   flip{};                   // our flip variable
    graphics::Graphic* myGraphic{};  // our graphic (if we choose to use one for a tick mark)

private:
    // slot table helper methods
    bool setSlotTickMarkLength(const base::INumber* const);
    bool setSlotQuantity(const base::Integer* const);
    bool setSlotGaugeLength(const base::INumber* const);
    bool setSlotFlip(const base::Boolean* const);
};

}
}

#endif
