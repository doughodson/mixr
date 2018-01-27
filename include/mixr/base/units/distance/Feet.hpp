
#ifndef __mixr_base_Feet_H__
#define __mixr_base_Feet_H__

#include "mixr/base/units/distance/Distance.hpp"

#include "mixr/base/units/util/distance_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Feet
// Description:  Meters / 0.3048
//------------------------------------------------------------------------------
class Feet final: public Distance
{
    DECLARE_SUBCLASS(Feet, Distance)

public:
    Feet();
    Feet(const double);
    Feet(const Distance&);

    static double convertStatic(const Distance& n)      { return n.toDistance() * distance::M2FT; }

    double toDistance() const final                     { return getValue() * distance::FT2M; }
    double fromDistance(const double a) const final     { return a * distance::M2FT; }
};

}
}

#endif
