
#include "mixr/base/units/time/MilliSeconds.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(MilliSeconds, "MilliSeconds")
EMPTY_SLOTTABLE(MilliSeconds)
EMPTY_COPYDATA(MilliSeconds)
EMPTY_DELETEDATA(MilliSeconds)

MilliSeconds::MilliSeconds() : Time()
{
    STANDARD_CONSTRUCTOR()
}

MilliSeconds::MilliSeconds(const double value) : Time(value)
{
    STANDARD_CONSTRUCTOR()
}

MilliSeconds::MilliSeconds(const Time& org) : Time()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org, true);
}

}
}
