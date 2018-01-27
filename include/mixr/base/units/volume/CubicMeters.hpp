
#ifndef __mixr_base_CubicMeters_H__
#define __mixr_base_CubicMeters_H__

#include "mixr/base/units/volume/Volume.hpp"

#include "mixr/base/units/util/volume_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: CubicMeters
// Description: An instance of CubicMeters with its value equal to 1.0 is one
// base unit for volume.
//------------------------------------------------------------------------------
class CubicMeters final: public Volume
{
    DECLARE_SUBCLASS(CubicMeters, Volume)

public:
    CubicMeters();
    CubicMeters(const double);
    CubicMeters(const Volume&);

    static double convertStatic(const Volume &n)      { return n.toVolume(); }

    double toVolume() const final                     { return getValue(); }
    double fromVolume(const double a) const final     { return a; }
};

}
}

#endif
