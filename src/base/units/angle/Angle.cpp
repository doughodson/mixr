
#include "mixr/base/units/angle/Angle.hpp"

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(Angle, "AbstractAngle")
EMPTY_SLOTTABLE(Angle)
EMPTY_DELETEDATA(Angle)

Angle::Angle()
{
   STANDARD_CONSTRUCTOR()
}

Angle::Angle(const double x) : Unit(x)
{
   STANDARD_CONSTRUCTOR()
}

void Angle::copyData(const Angle& org, const bool)
{
   BaseClass::copyData(org);
   setValue(fromAngle(org.toAngle()));
}

}
}
