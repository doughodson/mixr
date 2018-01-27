
#include "mixr/base/units/energy/KiloWattHours.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(KiloWattHours, "KiloWattHours")
EMPTY_SLOTTABLE(KiloWattHours)
EMPTY_COPYDATA(KiloWattHours)
EMPTY_DELETEDATA(KiloWattHours)

KiloWattHours::KiloWattHours() : Energy()
{
    STANDARD_CONSTRUCTOR()
}

KiloWattHours::KiloWattHours(const double value) : Energy(value)
{
    STANDARD_CONSTRUCTOR()
}

KiloWattHours::KiloWattHours(const Energy& value) : Energy()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value, true);
}

}
}

