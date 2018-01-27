
#include "mixr/base/units/distance/Meters.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Meters, "Meters")
EMPTY_SLOTTABLE(Meters)
EMPTY_COPYDATA(Meters)
EMPTY_DELETEDATA(Meters)

Meters::Meters() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

Meters::Meters(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

Meters::Meters(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
