
#ifndef __mixr_base_StatuteMiles_H__
#define __mixr_base_StatuteMiles_H__

#include "mixr/base/units/distance/Distance.hpp"

#include "mixr/base/units/util/distance_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: StatuteMiles
// Description:  Meters * 1609.34313095
//------------------------------------------------------------------------------
class StatuteMiles final: public Distance
{
    DECLARE_SUBCLASS(StatuteMiles, Distance)

public:
    StatuteMiles();
    StatuteMiles(const double);
    StatuteMiles(const Distance&);

    static double convertStatic(const Distance& n)      { return n.toDistance() * distance::M2SM; }

    double toDistance() const final                     { return getValue() * distance::SM2M; }
    double fromDistance(const double a) const final     { return a * distance::M2SM; }
};

}
}

#endif
