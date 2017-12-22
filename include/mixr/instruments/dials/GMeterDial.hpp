
#ifndef __mixr_instruments_GMeterDial_H__
#define __mixr_instruments_GMeterDial_H__

#include "mixr/instruments/dials/AnalogDial.hpp"

namespace mixr {
namespace instruments {

//------------------------------------------------------------------------------
// Class: GMeterDial
//
// Description: G meter, which simply keeps track of min and max and actual G's
//
// Inputs:
//      UPDATE_INSTRUMENTS (from instruments) -> g's
//      RESET_EVENT -> resets min and max g's
//------------------------------------------------------------------------------
class GMeterDial : public AnalogDial
{
    DECLARE_SUBCLASS(GMeterDial, AnalogDial)

public:
    GMeterDial();

    void drawFunc() override;
    void updateData(const double dt = 0.0) override;
    void reset() override;

private:
    double rotation {}; // our rotation angle (of the gauge arrow)
    double maxG {};     // max g value - both at 1 and -1 (on the dial)
    double minG {};     // min g value

    SendData gsSD;      // actual gs
    SendData minGSD;    // min gs
    SendData maxGSD;    // max gs
};

}
}

#endif
