
#include "mixr/base/units/area/SquareMilliMeters.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(SquareMilliMeters, "SquareMilliMeters")
EMPTY_SLOTTABLE(SquareMilliMeters)
EMPTY_COPYDATA(SquareMilliMeters)
EMPTY_DELETEDATA(SquareMilliMeters)

SquareMilliMeters::SquareMilliMeters() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareMilliMeters::SquareMilliMeters(const double value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareMilliMeters::SquareMilliMeters(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
