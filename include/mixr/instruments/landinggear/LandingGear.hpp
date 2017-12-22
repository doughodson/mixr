
#ifndef __mixr_instruments_LandingGear_H__
#define __mixr_instruments_LandingGear_H__

#include "mixr/instruments/Instrument.hpp"

namespace mixr {
namespace instruments {

//------------------------------------------------------------------------------
// Class: LandingGear
//
// Description: Generic Landing Gear readout
//
// Public member functions:
//      setGearDownValue() (units)
//      setGearUpValue()   (units)
//------------------------------------------------------------------------------
class LandingGear : public Instrument
{
    DECLARE_SUBCLASS(LandingGear,Instrument)

public:
    LandingGear();

    virtual bool setGearDownValue(const double);
    virtual bool setGearUpValue(const double);

    double getGearUpValue() const   { return gearUV; }
    double getGearDownValue() const { return gearDV; }
    int getGearState() const        { return gearState; }
    bool getInTransit() const       { return inTransit; }
    double getGearPos() const       { return gearPos; }
    bool getHaveRotary() const      { return haveRotary; }

    void drawFunc() override;

    void updateData(const double dt = 0.0) override;

private:
    int gearState {};      // is our gear down, up, or in transit?
    bool inTransit {};     // are we going from one commanded position to another?
    double gearPos {};     // our gear position
    double gearDV {1.0};   // our gear down indicator value
    double gearUV {};      // our gear up indicator value

    SendData gearSelSD;    // which gear position we are setting
    bool haveRotary {};    // do we have a rotary now that is our components (we won't draw if we do)

private:
    // slot table helper methods
    bool setSlotGearDownValue(const base::Number* const);
    bool setSlotGearUpValue(const base::Number* const);
};

}
}

#endif
