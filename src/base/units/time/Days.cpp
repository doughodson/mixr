
#include "mixr/base/units/time/Days.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Days, "Days")
EMPTY_SLOTTABLE(Days)
EMPTY_COPYDATA(Days)
EMPTY_DELETEDATA(Days)

Days::Days() : Time()
{
    STANDARD_CONSTRUCTOR()
}

Days::Days(const double value) : Time(value)
{
    STANDARD_CONSTRUCTOR()
}

Days::Days(const Time& org) : Time()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org, true);
}

}
}
