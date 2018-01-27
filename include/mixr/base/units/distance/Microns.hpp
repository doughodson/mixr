
#ifndef __mixr_base_Microns_H__
#define __mixr_base_Microns_H__

#include "mixr/base/units/distance/Distance.hpp"

#include "mixr/base/units/util/distance_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Microns(Same as MicroMeters)
// Description:  Meters * 1,000,000.0
//------------------------------------------------------------------------------
class Microns final: public Distance
{
    DECLARE_SUBCLASS(Microns, Distance)

public:
    Microns();
    Microns(const double);
    Microns(const Distance&);

    static double convertStatic(const Distance& n)       { return n.toDistance() * distance::M2UM; }

    double toDistance() const final                      { return getValue() * distance::UM2M; }
    double fromDistance(const double a) const final      { return a * distance::M2UM; }
};

}
}

#endif
