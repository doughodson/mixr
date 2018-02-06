
#include "mixr/base/units/angle/Degrees.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Degrees, "Degrees")
EMPTY_SLOTTABLE(Degrees)
EMPTY_COPYDATA(Degrees)
EMPTY_DELETEDATA(Degrees)
EMPTY_CONSTRUCTOR(Degrees)

Degrees::Degrees(const double x)
{
    STANDARD_CONSTRUCTOR()
    setValue(convertToBaseUnit(x));
}

}
}
