
#include "mixr/base/units/angle/Degrees.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Degrees, "Degrees")
EMPTY_SLOTTABLE(Degrees)
EMPTY_COPYDATA(Degrees)
EMPTY_DELETEDATA(Degrees)

Degrees::Degrees() : Angle()
{
    STANDARD_CONSTRUCTOR()
}

Degrees::Degrees(const double value) : Angle(value)
{
    STANDARD_CONSTRUCTOR()
}

Degrees::Degrees(const Angle& value) : Angle()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
