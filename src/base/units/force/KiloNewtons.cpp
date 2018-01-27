
#include "mixr/base/units/force/KiloNewtons.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(KiloNewtons, "KiloNewtons")
EMPTY_SLOTTABLE(KiloNewtons)
EMPTY_COPYDATA(KiloNewtons)
EMPTY_DELETEDATA(KiloNewtons)

KiloNewtons::KiloNewtons() : Force()
{
    STANDARD_CONSTRUCTOR()
}

KiloNewtons::KiloNewtons(const double value) : Force(value)
{
    STANDARD_CONSTRUCTOR()
}

KiloNewtons::KiloNewtons(const Force& value) : Force()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value, true);
}

}
}

