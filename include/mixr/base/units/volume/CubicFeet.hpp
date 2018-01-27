
#ifndef __mixr_base_CubicFeet_H__
#define __mixr_base_CubicFeet_H__

#include "mixr/base/units/volume/Volume.hpp"

#include "mixr/base/units/util/volume_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: CubicFeet
// Description: Cubic Meters * 35.31467
//------------------------------------------------------------------------------
class CubicFeet final: public Volume
{
    DECLARE_SUBCLASS(CubicFeet, Volume)

public:
    CubicFeet();
    CubicFeet(const double);
    CubicFeet(const Volume&);

    static double convertStatic(const Volume &n)     { return n.toVolume() * volume::CM2CFT; }

    double toVolume() const final                    { return getValue() * volume::CFT2CM; }
    double fromVolume(const double a) const final    { return a * volume::CM2CFT; }
};

}
}

#endif
