
#include "mixr/base/units/time/Hours.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Hours, "Hours")
EMPTY_SLOTTABLE(Hours)
EMPTY_COPYDATA(Hours)
EMPTY_DELETEDATA(Hours)

Hours::Hours() : Time()
{
    STANDARD_CONSTRUCTOR()
}

Hours::Hours(const double value) : Time(value)
{
    STANDARD_CONSTRUCTOR()
}

Hours::Hours(const Time& org) : Time()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org, true);
}

}
}
