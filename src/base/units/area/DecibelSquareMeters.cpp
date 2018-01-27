
#include "mixr/base/units/area/DecibelSquareMeters.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(DecibelSquareMeters, "DecibelSquareMeters")
EMPTY_SLOTTABLE(DecibelSquareMeters)
EMPTY_COPYDATA(DecibelSquareMeters)
EMPTY_DELETEDATA(DecibelSquareMeters)

DecibelSquareMeters::DecibelSquareMeters() : Area()
{
    STANDARD_CONSTRUCTOR()
}

DecibelSquareMeters::DecibelSquareMeters(const double value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

DecibelSquareMeters::DecibelSquareMeters(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
