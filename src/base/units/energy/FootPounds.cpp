
#include "mixr/base/units/energy/FootPounds.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(FootPounds, "FootPounds")
EMPTY_SLOTTABLE(FootPounds)
EMPTY_COPYDATA(FootPounds)
EMPTY_DELETEDATA(FootPounds)

FootPounds::FootPounds() : Energy()
{
    STANDARD_CONSTRUCTOR()
}

FootPounds::FootPounds(const double value) : Energy(value)
{
    STANDARD_CONSTRUCTOR()
}

FootPounds::FootPounds(const Energy& value) : Energy()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value, true);
}

}
}

