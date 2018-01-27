
#include "mixr/base/units/power/DecibelMilliWatts.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(DecibelMilliWatts, "DecibelMilliWatts")
EMPTY_SLOTTABLE(DecibelMilliWatts)
EMPTY_COPYDATA(DecibelMilliWatts)
EMPTY_DELETEDATA(DecibelMilliWatts)

DecibelMilliWatts::DecibelMilliWatts() : Power()
{
    STANDARD_CONSTRUCTOR()
}

DecibelMilliWatts::DecibelMilliWatts(const double value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

DecibelMilliWatts::DecibelMilliWatts(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org, true);
}

}
}
