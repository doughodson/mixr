
#ifndef __mixr_instruments_DialPointer_H__
#define __mixr_instruments_DialPointer_H__

#include "mixr/instruments/dials/AnalogDial.hpp"

namespace mixr {
namespace instruments {

//------------------------------------------------------------------------------
// Class: DialPointer
//
// Description: Pointer that will spin around a defined arc or circle at a defined
// scaled rate.
// Input:  UPDATE_INSTRUMENTS (from instruments)
//------------------------------------------------------------------------------
class DialPointer : public AnalogDial
{
    DECLARE_SUBCLASS(DialPointer, AnalogDial)

public:
    DialPointer();

    void draw() override;
};

}
}

#endif
