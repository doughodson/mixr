
#include "mixr/base/units/mass/Slugs.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Slugs, "Slugs")
EMPTY_SLOTTABLE(Slugs)
EMPTY_COPYDATA(Slugs)
EMPTY_DELETEDATA(Slugs)

Slugs::Slugs() : Mass()
{
    STANDARD_CONSTRUCTOR()
}

Slugs::Slugs(const double value) : Mass(value)
{
    STANDARD_CONSTRUCTOR()
}

Slugs::Slugs(const Mass& value) : Mass()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value, true);
}

}
}
