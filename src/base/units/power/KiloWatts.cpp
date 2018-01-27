
#include "mixr/base/units/power/KiloWatts.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(KiloWatts, "KiloWatts")
EMPTY_SLOTTABLE(KiloWatts)
EMPTY_COPYDATA(KiloWatts)
EMPTY_DELETEDATA(KiloWatts)

KiloWatts::KiloWatts() : Power()
{
    STANDARD_CONSTRUCTOR()
}

KiloWatts::KiloWatts(const double value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

KiloWatts::KiloWatts(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org, true);
}

}
}
