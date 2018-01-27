
#include "mixr/base/units/force/Poundals.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Poundals, "Poundals")
EMPTY_SLOTTABLE(Poundals)
EMPTY_COPYDATA(Poundals)
EMPTY_DELETEDATA(Poundals)

Poundals::Poundals() : Force()
{
    STANDARD_CONSTRUCTOR()
}

Poundals::Poundals(const double value) : Force(value)
{
    STANDARD_CONSTRUCTOR()
}

Poundals::Poundals(const Force& value) : Force()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value, true);
}

}
}

