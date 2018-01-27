
#include "mixr/base/units/power/Watts.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Watts, "Watts")
EMPTY_SLOTTABLE(Watts)
EMPTY_COPYDATA(Watts)
EMPTY_DELETEDATA(Watts)

Watts::Watts() : Power()
{
    STANDARD_CONSTRUCTOR()
}

Watts::Watts(const double value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

Watts::Watts(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org, true);
}

}
}
