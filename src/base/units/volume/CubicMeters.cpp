
#include "mixr/base/units/volume/CubicMeters.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(CubicMeters, "CubicMeters")
EMPTY_SLOTTABLE(CubicMeters)
EMPTY_COPYDATA(CubicMeters)
EMPTY_DELETEDATA(CubicMeters)

CubicMeters::CubicMeters() : Volume()
{
    STANDARD_CONSTRUCTOR()
}

CubicMeters::CubicMeters(const double value) : Volume(value)
{
    STANDARD_CONSTRUCTOR()
}

CubicMeters::CubicMeters(const Volume& value) : Volume()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}
   

}
}

