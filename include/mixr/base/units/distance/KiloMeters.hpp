
#ifndef __mixr_base_KiloMeters_H__
#define __mixr_base_KiloMeters_H__

#include "mixr/base/units/distance/Distance.hpp"

#include "mixr/base/units/util/distance_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: KiloMeters
// Description:  Meters / 1000.0
//------------------------------------------------------------------------------
class KiloMeters final: public Distance
{
    DECLARE_SUBCLASS(KiloMeters, Distance)

public:
    KiloMeters();
    KiloMeters(const double);
    KiloMeters(const Distance&);

    static double convertStatic(const Distance& n)       { return n.toDistance() * distance::M2KM; }

    double toDistance() const final                      { return getValue() * distance::KM2M; }
    double fromDistance(const double a) const final      { return a * distance::M2KM; }
};

}
}

#endif
