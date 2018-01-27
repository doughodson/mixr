
#ifndef __mixr_base_Liters_H__
#define __mixr_base_Liters_H__

#include "mixr/base/units/volume/Volume.hpp"

#include "mixr/base/units/util/volume_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Liters
// Description: Cubic Meters * 1000
//------------------------------------------------------------------------------
class Liters final: public Volume
{
    DECLARE_SUBCLASS(Liters, Volume)

public:
    Liters();
    Liters(const double);
    Liters(const Volume&);

    static double convertStatic(const Volume &n)     { return n.toVolume() * volume::CM2L; }

    double toVolume() const                          { return getValue() * volume::L2CM; }
    double fromVolume(const double a) const          { return a * volume::CM2L; }
};

}
}

#endif
