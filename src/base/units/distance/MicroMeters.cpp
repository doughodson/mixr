
#include "mixr/base/units/distance/MicroMeters.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(MicroMeters, "MicroMeters")
EMPTY_SLOTTABLE(MicroMeters)
EMPTY_COPYDATA(MicroMeters)
EMPTY_DELETEDATA(MicroMeters)

MicroMeters::MicroMeters() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

MicroMeters::MicroMeters(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

MicroMeters::MicroMeters(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
