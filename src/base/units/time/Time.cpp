
#include "mixr/base/units/time/Time.hpp"

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(Time, "AbstractTime")
EMPTY_SLOTTABLE(Time)
EMPTY_DELETEDATA(Time)

Time::Time()
{
    STANDARD_CONSTRUCTOR()
}

Time::Time(const double value) : Unit(value)
{
    STANDARD_CONSTRUCTOR()
}

void Time::copyData(const Time& org, const bool)
{
   BaseClass::copyData(org);
   setValue(fromTime(org.toTime()));
}

}
}
