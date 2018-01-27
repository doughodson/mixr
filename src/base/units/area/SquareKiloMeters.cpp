
#include "mixr/base/units/area/SquareKiloMeters.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(SquareKiloMeters, "SquareKiloMeters")
EMPTY_SLOTTABLE(SquareKiloMeters)
EMPTY_COPYDATA(SquareKiloMeters)
EMPTY_DELETEDATA(SquareKiloMeters)

SquareKiloMeters::SquareKiloMeters() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareKiloMeters::SquareKiloMeters(const double value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareKiloMeters::SquareKiloMeters(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
