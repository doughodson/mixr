
#include "mixr/base/qty/angle/Radians.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Radians, "Radians")
EMPTY_SLOTTABLE(Radians)
EMPTY_COPYDATA(Radians)
EMPTY_DELETEDATA(Radians)
EMPTY_CONSTRUCTOR(Radians)

Radians::Radians(const double x)
{
    STANDARD_CONSTRUCTOR()
    setValue(convertToBaseUnit(x));
}

}
}
