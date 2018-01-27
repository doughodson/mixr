
#include "mixr/base/units/mass/Mass.hpp"

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(Mass, "AbstractMass")
EMPTY_SLOTTABLE(Mass)
EMPTY_DELETEDATA(Mass)

Mass::Mass()
{
    STANDARD_CONSTRUCTOR()
}

Mass::Mass(const double value) : Unit(value)
{
    STANDARD_CONSTRUCTOR()
}

void Mass::copyData(const Mass& org, const bool)
{
    BaseClass::copyData(org);
    setValue(fromMass(org.toMass()));
}

}
}
