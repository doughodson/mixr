
#ifndef __mixr_base_PoundForces_H__
#define __mixr_base_PoundForces_H__

#include "mixr/base/units/force/Force.hpp"

#include "mixr/base/units/util/force_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: PoundForces
// Description:  Newtons * 0.224809
//------------------------------------------------------------------------------
class PoundForces final: public Force
{
    DECLARE_SUBCLASS(PoundForces, Force)

public:
    PoundForces();
    PoundForces(const double);
    PoundForces(const Force&);

    static double convertStatic(const Force& n)        { return n.toForce() * force::N2PF; }

    double toForce() const final                       { return getValue() * force::PF2N; }
    double fromForce(const double a) const final       { return a * force::N2PF; }
};

}
}

#endif
