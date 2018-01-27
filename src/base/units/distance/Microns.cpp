
#include "mixr/base/units/distance/Microns.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Microns, "Microns")
EMPTY_SLOTTABLE(Microns)
EMPTY_COPYDATA(Microns)
EMPTY_DELETEDATA(Microns)

Microns::Microns() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

Microns::Microns(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

Microns::Microns(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
