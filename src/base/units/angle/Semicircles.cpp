
#include "mixr/base/units/angle/Semicircles.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Semicircles, "Semicircles")
EMPTY_SLOTTABLE(Semicircles)
EMPTY_COPYDATA(Semicircles)
EMPTY_DELETEDATA(Semicircles)
EMPTY_CONSTRUCTOR(Semicircles)

Semicircles::Semicircles(const double x)
{
    STANDARD_CONSTRUCTOR()
    setValue(convertToBaseUnit(x));
}

}
}
