
#ifndef __mixr_base_KiloNewtons_H__
#define __mixr_base_KiloNewtons_H__

#include "mixr/base/units/force/Force.hpp"

#include "mixr/base/units/util/force_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: KiloNewtons
// Description:  Newtons * 1000.0
//------------------------------------------------------------------------------
class KiloNewtons final: public Force
{
    DECLARE_SUBCLASS(KiloNewtons, Force)

public:
    KiloNewtons();
    KiloNewtons(const double);
    KiloNewtons(const Force&);

    static double convertStatic(const Force& n)        { return n.toForce() * force::N2KN; }

    double toForce() const final                       { return getValue() * force::KN2N; }
    double fromForce(const double a) const final       { return a * force::N2KN; }
};

}
}

#endif
