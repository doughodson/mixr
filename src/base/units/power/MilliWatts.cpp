
#include "mixr/base/units/power/MilliWatts.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(MilliWatts, "MilliWatts")
EMPTY_SLOTTABLE(MilliWatts)
EMPTY_COPYDATA(MilliWatts)
EMPTY_DELETEDATA(MilliWatts)

MilliWatts::MilliWatts() : Power()
{
    STANDARD_CONSTRUCTOR()
}

MilliWatts::MilliWatts(const double value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

MilliWatts::MilliWatts(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org, true);
}

}
}
