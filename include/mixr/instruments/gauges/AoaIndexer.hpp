
#ifndef __mixr_instruments_AoAIndexer_H__
#define __mixr_instruments_AoAIndexer_H__

#include "mixr/instruments/Instrument.hpp"

namespace mixr {
namespace instruments {

//------------------------------------------------------------------------------
// Class: AoAIndexer
//
// Description: Generic AOA gauge that, given limits, will indicate over, under,
// and in-range aoa values.
//
// Inputs:
//      UPDATE_INSTRUMENTS token = actual aoa
//      All the rest are slots (red min, max, yellow min, max, and green min, max)
//      with:
//      red -> over limit area
//      green -> in limit area
//      yellow -> under limit area
//------------------------------------------------------------------------------
class AoAIndexer : public Instrument
{
    DECLARE_SUBCLASS(AoAIndexer,Instrument)

public:
    AoAIndexer();

    virtual bool setAoaRedMin(const double);
    virtual bool setAoaRedMax(const double);
    virtual bool setAoaYellowMin(const double);
    virtual bool setAoaYellowMax(const double);
    virtual bool setAoaGreenMin(const double);
    virtual bool setAoaGreenMax(const double);

    double getAoaRedMin() const     { return aoaRedMin; }
    double getAoaRedMax() const     { return aoaRedMax; }
    double getAoaYellowMin() const  { return aoaYellowMin; }
    double getAoaYellowMax() const  { return aoaYellowMax; }
    double getAoaGreenMin() const   { return aoaGreenMin; }
    double getAoaGreenMax() const   { return aoaGreenMax; }
    int    getState() const         { return aoaState; }

    void drawFunc() override;

    void updateData(const double dt = 0) override;

private:

//  --  -> aoaMax
//
//  --  -> aoaZero
//
//  --  -> aoaMin

    double aoaRedMin {10};       // min value of where the red portion of our aoa lights up
    double aoaRedMax {15};       // max value of where the red portion of our aoa lights up
    double aoaYellowMin {-10};   // min value of where the yellow portion of our aoa lights up
    double aoaYellowMax {-5};    // max value of where the yellow portion of our aoa lights up
    double aoaGreenMin {-5};     // min value of where the green portion of our aoa lights up
    double aoaGreenMax {10};     // max value of where the green portion of our aoa lights up

    GLuint displayList {};  // our display list name
    bool isDlist {};        // do we have a display list?
    int aoaState {2};       // shows which state we are in
    SendData selectSD;      // which graphics are we selecting? (If we are using a rotary)

private:
    // slot table helper methods
    bool setSlotAoaRedMin(const base::Number* const);
    bool setSlotAoaRedMax(const base::Number* const);
    bool setSlotAoaYellowMin(const base::Number* const);
    bool setSlotAoaYellowMax(const base::Number* const);
    bool setSlotAoaGreenMax(const base::Number* const);
    bool setSlotAoaGreenMin(const base::Number* const);
};

}
}

#endif
