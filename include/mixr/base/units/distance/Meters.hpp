
#ifndef __mixr_base_Meters_H__
#define __mixr_base_Meters_H__

#include "mixr/base/units/distance/Distance.hpp"

#include "mixr/base/units/util/distance_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Meters
// Description:  An instance of Meters with its value equal to 1.0 is one
//               base unit for distances.
//------------------------------------------------------------------------------
class Meters final: public Distance
{
    DECLARE_SUBCLASS(Meters, Distance)

public:
    Meters();
    Meters(const double);
    Meters(const Distance&);

    static double convertStatic(const Distance& n)                { return n.toDistance(); }

    double toDistance() const final                               { return getValue(); }
    double fromDistance(const double a) const final               { return a; }
};

}
}

#endif
