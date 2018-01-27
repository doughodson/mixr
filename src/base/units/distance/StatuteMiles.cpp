
#include "mixr/base/units/distance/StatuteMiles.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(StatuteMiles, "StatuteMiles")
EMPTY_SLOTTABLE(StatuteMiles)
EMPTY_COPYDATA(StatuteMiles)
EMPTY_DELETEDATA(StatuteMiles)

StatuteMiles::StatuteMiles() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

StatuteMiles::StatuteMiles(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

StatuteMiles::StatuteMiles(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
