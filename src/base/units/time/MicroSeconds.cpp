
#include "mixr/base/units/time/MicroSeconds.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(MicroSeconds, "MicroSeconds")
EMPTY_SLOTTABLE(MicroSeconds)
EMPTY_COPYDATA(MicroSeconds)
EMPTY_DELETEDATA(MicroSeconds)

MicroSeconds::MicroSeconds() : Time()
{
    STANDARD_CONSTRUCTOR()
}

MicroSeconds::MicroSeconds(const double value) : Time(value)
{
    STANDARD_CONSTRUCTOR()
}

MicroSeconds::MicroSeconds(const Time& org) : Time()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org, true);
}

}
}
