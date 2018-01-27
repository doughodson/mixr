
#include "mixr/base/units/force/Force.hpp"

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(Force, "AbstractForce")
EMPTY_SLOTTABLE(Force)
EMPTY_DELETEDATA(Force)

Force::Force()
{
    STANDARD_CONSTRUCTOR()
}

Force::Force(const double value) : Unit(value)
{
    STANDARD_CONSTRUCTOR()
}

void Force::copyData(const Force& org, const bool)
{
   BaseClass::copyData(org);
   setValue(fromForce(org.toForce()));
}

}
}

