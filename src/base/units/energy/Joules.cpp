
#include "mixr/base/units/energy/Joules.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Joules, "Joules")
EMPTY_SLOTTABLE(Joules)
EMPTY_COPYDATA(Joules)
EMPTY_DELETEDATA(Joules)

Joules::Joules() : Energy()
{
    STANDARD_CONSTRUCTOR()
}

Joules::Joules(const double value) : Energy(value)
{
    STANDARD_CONSTRUCTOR()
}

Joules::Joules(const Energy& value) : Energy()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value, true);
}

}
}

