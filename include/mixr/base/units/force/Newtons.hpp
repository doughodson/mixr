
#ifndef __mixr_base_Newtons_H__
#define __mixr_base_Newtons_H__

#include "mixr/base/units/force/Force.hpp"

#include "mixr/base/units/util/force_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Newtons
// Description:  An instance of Newtons with its value equal to 1.0 is one
//               base unit for forces.
//------------------------------------------------------------------------------
class Newtons final: public Force
{
    DECLARE_SUBCLASS(Newtons, Force)

public:
    Newtons();
    Newtons(const double);
    Newtons(const Force&);

    static double convertStatic(const Force& n)        { return n.toForce(); }

    double toForce() const final                       { return getValue(); }
    double fromForce(const double a) const final       { return a; }
};

}
}

#endif
