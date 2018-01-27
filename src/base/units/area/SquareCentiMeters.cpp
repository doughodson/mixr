
#include "mixr/base/units/area/SquareCentiMeters.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(SquareCentiMeters, "SquareCentiMeters")
EMPTY_SLOTTABLE(SquareCentiMeters)
EMPTY_COPYDATA(SquareCentiMeters)
EMPTY_DELETEDATA(SquareCentiMeters)

SquareCentiMeters::SquareCentiMeters() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareCentiMeters::SquareCentiMeters(const double value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareCentiMeters::SquareCentiMeters(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value, true);
}

}
}
