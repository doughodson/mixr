
#include "mixr/base/units/power/Power.hpp"

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(Power, "Power")
EMPTY_SLOTTABLE(Power)
EMPTY_DELETEDATA(Power)

Power::Power()
{
   STANDARD_CONSTRUCTOR()
}

Power::Power(const double value) : Unit(value)
{
   STANDARD_CONSTRUCTOR()
}

void Power::copyData(const Power& org, const bool)
{
   BaseClass::copyData(org);
   setValue(fromPower(org.toPower()));
}

}
}
