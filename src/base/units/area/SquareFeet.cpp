
#include "mixr/base/units/area/SquareFeet.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(SquareFeet, "SquareFeet")
EMPTY_SLOTTABLE(SquareFeet)
EMPTY_COPYDATA(SquareFeet)
EMPTY_DELETEDATA(SquareFeet)

SquareFeet::SquareFeet() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareFeet::SquareFeet(const double value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareFeet::SquareFeet(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value, true);
}

}
}
