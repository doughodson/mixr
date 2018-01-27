
#ifndef __mixr_base_NauticalMiles_H__
#define __mixr_base_NauticalMiles_H__

#include "mixr/base/units/distance/Distance.hpp"

#include "mixr/base/units/util/distance_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: NauticalMiles
// Description:  Meters * 1851.999942
//------------------------------------------------------------------------------
class NauticalMiles final: public Distance
{
    DECLARE_SUBCLASS(NauticalMiles, Distance)

public:
    NauticalMiles();
    NauticalMiles(const double);
    NauticalMiles(const Distance&);

    static double convertStatic(const Distance& n)     { return n.toDistance() * distance::M2NM; }

    double toDistance() const final                    { return getValue() * distance::NM2M; }
    double fromDistance(const double a) const final    { return a * distance::M2NM; }
};

}
}

#endif
