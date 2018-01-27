
#include "mixr/base/units/energy/Energy.hpp"

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(Energy, "AbstractEnergy")
EMPTY_SLOTTABLE(Energy)
EMPTY_DELETEDATA(Energy)

Energy::Energy()
{
    STANDARD_CONSTRUCTOR()
}

Energy::Energy(const double value) : Unit(value)
{
    STANDARD_CONSTRUCTOR()
}

void Energy::copyData(const Energy& org, const bool)
{
   BaseClass::copyData(org);
   setValue(fromEnergy(org.toEnergy()));
}

}
}

