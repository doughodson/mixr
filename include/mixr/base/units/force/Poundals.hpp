
#ifndef __mixr_base_Poundals_H__
#define __mixr_base_Poundals_H__

#include "mixr/base/units/force/Force.hpp"

#include "mixr/base/units/util/force_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Poundals
// Description:  Newtons * 7.23301
//------------------------------------------------------------------------------
class Poundals final: public Force
{
    DECLARE_SUBCLASS(Poundals, Force)

public:
    Poundals();
    Poundals(const double);
    Poundals(const Force&);

    static double convertStatic(const Force& n)        { return n.toForce() * force::N2PD; }

    double toForce() const final                       { return getValue() * force::PD2N; }
    double fromForce(const double a) const final       { return a * force::N2PD; }
};

}
}

#endif
