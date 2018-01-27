
#ifndef __mixr_base_CentiMeters_H__
#define __mixr_base_CentiMeters_H__

#include "mixr/base/units/distance/Distance.hpp"

#include "mixr/base/units/util/distance_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: CentiMeters
// Description:  Meters * 100.0
//------------------------------------------------------------------------------
class CentiMeters final: public Distance
{
    DECLARE_SUBCLASS(CentiMeters, Distance)

public:
    CentiMeters();
    CentiMeters(const double);
    CentiMeters(const Distance&);

    static double convertStatic(const Distance& n)        { return n.toDistance() * distance::M2CM; }

    double toDistance() const final                       { return getValue() * distance::CM2M; }
    double fromDistance(const double a) const final       { return a * distance::M2CM; }
};

}
}

#endif
