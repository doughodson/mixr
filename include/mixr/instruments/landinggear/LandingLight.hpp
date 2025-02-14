
#ifndef __mixr_instruments_LandingLight_HPP__
#define __mixr_instruments_LandingLight_HPP__

#include "LandingGear.hpp"

namespace mixr {
namespace base { class Identifier; }
namespace instruments {

//------------------------------------------------------------------------------
// Class: LandingLight
//
// Description: Generic landing gear lights, which use the same principle as
// the landing gear, only they don't show a transitional phase, just off
// or on.
//
// Public member functions:
//      setLightRadius() - sets the radius (inches) of the light
//------------------------------------------------------------------------------
class LandingLight : public LandingGear
{
    DECLARE_SUBCLASS(LandingLight,LandingGear)

public:
    LandingLight();

    double getLightRadius() const           { return lRadius; }

    virtual bool setLightRadius(const double);

    void updateData(const double dt = 0.0) override;

    void drawFunc() override;

private:
    double lRadius{};      // our light radius - 0 means don't use it
    double gearCurrent{};  // our gear current value
    SendData selSD;        // our selection

private:
    // slot table helper methods
    bool setSlotLightRadius(const base::INumber* const);
};

}
}

#endif
