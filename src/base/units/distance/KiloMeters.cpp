
#include "mixr/base/units/distance/KiloMeters.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(KiloMeters, "KiloMeters")
EMPTY_SLOTTABLE(KiloMeters)
EMPTY_COPYDATA(KiloMeters)
EMPTY_DELETEDATA(KiloMeters)

KiloMeters::KiloMeters() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

KiloMeters::KiloMeters(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

KiloMeters::KiloMeters(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
