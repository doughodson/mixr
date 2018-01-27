
#include "mixr/base/units/area/SquareMeters.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(SquareMeters, "SquareMeters")
EMPTY_SLOTTABLE(SquareMeters)
EMPTY_COPYDATA(SquareMeters)
EMPTY_DELETEDATA(SquareMeters)

SquareMeters::SquareMeters() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareMeters::SquareMeters(const double value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareMeters::SquareMeters(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
