
#include "mixr/base/units/area/SquareYards.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(SquareYards, "SquareYards")
EMPTY_SLOTTABLE(SquareYards)
EMPTY_COPYDATA(SquareYards)
EMPTY_DELETEDATA(SquareYards)

SquareYards::SquareYards() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareYards::SquareYards(const double value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareYards::SquareYards(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
