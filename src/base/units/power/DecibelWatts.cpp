
#include "mixr/base/units/power/DecibelWatts.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(DecibelWatts, "DecibelWatts")
EMPTY_SLOTTABLE(DecibelWatts)
EMPTY_COPYDATA(DecibelWatts)
EMPTY_DELETEDATA(DecibelWatts)

DecibelWatts::DecibelWatts() : Power()
{
    STANDARD_CONSTRUCTOR()
}

DecibelWatts::DecibelWatts(const double value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

DecibelWatts::DecibelWatts(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org, true);
}

}
}
