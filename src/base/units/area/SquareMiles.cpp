
#include "mixr/base/units/area/SquareMiles.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(SquareMiles, "SquareMiles")
EMPTY_SLOTTABLE(SquareMiles)
EMPTY_COPYDATA(SquareMiles)
EMPTY_DELETEDATA(SquareMiles)

SquareMiles::SquareMiles() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareMiles::SquareMiles(const double value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareMiles::SquareMiles(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
