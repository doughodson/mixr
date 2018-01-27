
#include "mixr/base/units/time/Minutes.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Minutes, "Minutes")
EMPTY_SLOTTABLE(Minutes)
EMPTY_COPYDATA(Minutes)
EMPTY_DELETEDATA(Minutes)

Minutes::Minutes() : Time()
{
    STANDARD_CONSTRUCTOR()
}

Minutes::Minutes(const double value) : Time(value)
{
    STANDARD_CONSTRUCTOR()
}

Minutes::Minutes(const Time& org) : Time()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org, true);
}

}
}
