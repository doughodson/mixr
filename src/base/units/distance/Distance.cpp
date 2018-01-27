
#include "mixr/base/units/distance/Distance.hpp"

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(Distance, "AbstractDistance")
EMPTY_SLOTTABLE(Distance)
EMPTY_DELETEDATA(Distance)

Distance::Distance()
{
    STANDARD_CONSTRUCTOR()
}

Distance::Distance(const double value) : Unit(value)
{
    STANDARD_CONSTRUCTOR()
}

void Distance::copyData(const Distance& org, const bool)
{
   BaseClass::copyData(org);
   setValue(fromDistance(org.toDistance()));
}

}
}
