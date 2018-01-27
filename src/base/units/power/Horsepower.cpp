
#include "mixr/base/units/power/Horsepower.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Horsepower, "Horsepower")
EMPTY_SLOTTABLE(Horsepower)
EMPTY_COPYDATA(Horsepower)
EMPTY_DELETEDATA(Horsepower)

Horsepower::Horsepower() : Power()
{
    STANDARD_CONSTRUCTOR()
}

Horsepower::Horsepower(const double value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

Horsepower::Horsepower(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org, true);
}

}
}
