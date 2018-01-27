
#include "mixr/base/units/distance/NauticalMiles.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(NauticalMiles, "NauticalMiles")
EMPTY_SLOTTABLE(NauticalMiles)
EMPTY_COPYDATA(NauticalMiles)
EMPTY_DELETEDATA(NauticalMiles)

NauticalMiles::NauticalMiles() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

NauticalMiles::NauticalMiles(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

NauticalMiles::NauticalMiles(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value, true);
}

}
}
