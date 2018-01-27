
#include "mixr/base/units/area/SquareInches.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(SquareInches, "SquareInches")
EMPTY_SLOTTABLE(SquareInches)
EMPTY_COPYDATA(SquareInches)
EMPTY_DELETEDATA(SquareInches)

SquareInches::SquareInches() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareInches::SquareInches(const double value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareInches::SquareInches(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
