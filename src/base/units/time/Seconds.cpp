
#include "mixr/base/units/time/Seconds.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Seconds, "Seconds")
EMPTY_SLOTTABLE(Seconds)
EMPTY_COPYDATA(Seconds)
EMPTY_DELETEDATA(Seconds)

Seconds::Seconds() : Time()
{
    STANDARD_CONSTRUCTOR()
}

Seconds::Seconds(const double value) : Time(value)
{
    STANDARD_CONSTRUCTOR()
}

Seconds::Seconds(const Time& org) : Time()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

}
}
