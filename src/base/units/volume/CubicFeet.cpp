
#include "mixr/base/units/volume/CubicFeet.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(CubicFeet, "CubicFeet")
EMPTY_SLOTTABLE(CubicFeet)
EMPTY_COPYDATA(CubicFeet)
EMPTY_DELETEDATA(CubicFeet)

CubicFeet::CubicFeet() : Volume()
{
    STANDARD_CONSTRUCTOR()
}

CubicFeet::CubicFeet(const double value) : Volume(value)
{
    STANDARD_CONSTRUCTOR()
}
CubicFeet::CubicFeet(const Volume& value) : Volume()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}

