
#include "mixr/base/units/time/NanoSeconds.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(NanoSeconds, "NanoSeconds")
EMPTY_SLOTTABLE(NanoSeconds)
EMPTY_COPYDATA(NanoSeconds)
EMPTY_DELETEDATA(NanoSeconds)

NanoSeconds::NanoSeconds() : Time()
{
    STANDARD_CONSTRUCTOR()
}

NanoSeconds::NanoSeconds(const double value) : Time(value)
{
    STANDARD_CONSTRUCTOR()
}

NanoSeconds::NanoSeconds(const Time& org) : Time()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org, true);
}

}
}
