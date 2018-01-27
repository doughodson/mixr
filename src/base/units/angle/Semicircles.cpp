
#include "mixr/base/units/angle/Semicircles.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Semicircles, "Semicircles")
EMPTY_SLOTTABLE(Semicircles)
EMPTY_COPYDATA(Semicircles)
EMPTY_DELETEDATA(Semicircles)

Semicircles::Semicircles() : Angle()
{
    STANDARD_CONSTRUCTOR()
}

Semicircles::Semicircles(const double value) : Angle(value)
{
    STANDARD_CONSTRUCTOR()
}

Semicircles::Semicircles(const Angle& value) : Angle()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
