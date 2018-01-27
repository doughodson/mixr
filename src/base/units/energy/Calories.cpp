
#include "mixr/base/units/energy/Calories.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Calories, "Calories")
EMPTY_SLOTTABLE(Calories)
EMPTY_COPYDATA(Calories)
EMPTY_DELETEDATA(Calories)

Calories::Calories() : Energy()
{
    STANDARD_CONSTRUCTOR()
}


Calories::Calories(const double value) : Energy(value)
{
    STANDARD_CONSTRUCTOR()
}

Calories::Calories(const Energy& value) : Energy()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value, true);
}

}
}

