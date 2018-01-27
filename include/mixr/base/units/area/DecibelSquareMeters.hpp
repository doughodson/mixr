
#ifndef __mixr_base_DecibelSquareMeters_H__
#define __mixr_base_DecibelSquareMeters_H__

#include "mixr/base/units/area/Area.hpp"

#include "mixr/base/units/util/area_utils.hpp"

#include <cmath>

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: DecibelSquareMeters
// Description: 10 * Log(Square Meters)
//------------------------------------------------------------------------------
class DecibelSquareMeters final: public Area
{
    DECLARE_SUBCLASS(DecibelSquareMeters, Area)

public:
    DecibelSquareMeters();
    DecibelSquareMeters(const double);
    DecibelSquareMeters(const Area&);

    static double convertStatic(const Area& n)     { return 10.0f * std::log10( n.toArea() ); }

    double toArea() const final                    { return std::pow(10.0, getValue()/10.0); }
    double fromArea(const double a) const final    { return 10.0f * std::log10(a); }
};

}
}

#endif
