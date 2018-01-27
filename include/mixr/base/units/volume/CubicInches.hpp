
#ifndef __mixr_base_CubicInches_H__
#define __mixr_base_CubicInches_H__

#include "mixr/base/units/volume/Volume.hpp"

#include "mixr/base/units/util/volume_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: CubicInches
// Description: Cubic Meters * 61023.74
//------------------------------------------------------------------------------
class CubicInches final: public Volume
{
    DECLARE_SUBCLASS(CubicInches, Volume)

public:
    CubicInches();
    CubicInches(const double);
    CubicInches(const Volume&);

    static double convertStatic(const Volume &n)     { return n.toVolume() * volume::CM2CIN; }

    double toVolume() const final                    { return getValue() * volume::CIN2CM; }
    double fromVolume(const double a) const final    { return a * volume::CM2CIN; }
};

}
}

#endif
