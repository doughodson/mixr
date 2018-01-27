
#include "mixr/base/units/angle/Radians.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Radians, "Radians")
EMPTY_SLOTTABLE(Radians)
EMPTY_COPYDATA(Radians)
EMPTY_DELETEDATA(Radians)

Radians::Radians() : Angle()
{
    STANDARD_CONSTRUCTOR()
}

Radians::Radians(const double value) : Angle(value)
{
    STANDARD_CONSTRUCTOR()
}

Radians::Radians(const Angle& value) : Angle()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
