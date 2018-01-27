
#include "mixr/base/units/distance/CentiMeters.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(CentiMeters, "CentiMeters")
EMPTY_SLOTTABLE(CentiMeters)
EMPTY_COPYDATA(CentiMeters)
EMPTY_DELETEDATA(CentiMeters)

CentiMeters::CentiMeters() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

CentiMeters::CentiMeters(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

CentiMeters::CentiMeters(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
