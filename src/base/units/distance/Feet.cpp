
#include "mixr/base/units/distance/Feet.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Feet, "Feet")
EMPTY_SLOTTABLE(Feet)
EMPTY_COPYDATA(Feet)
EMPTY_DELETEDATA(Feet)

Feet::Feet() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

Feet::Feet(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

Feet::Feet(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
