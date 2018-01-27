
#include "mixr/base/units/volume/CubicInches.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(CubicInches, "CubicInches")
EMPTY_SLOTTABLE(CubicInches)
EMPTY_COPYDATA(CubicInches)
EMPTY_DELETEDATA(CubicInches)

CubicInches::CubicInches() : Volume()
{
    STANDARD_CONSTRUCTOR()
}

CubicInches::CubicInches(const double value) : Volume(value)
{
    STANDARD_CONSTRUCTOR()
}
CubicInches::CubicInches(const Volume& value) : Volume()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}

