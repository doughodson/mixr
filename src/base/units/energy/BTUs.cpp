
#include "mixr/base/units/energy/BTUs.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(BTUs, "BTUs")
EMPTY_SLOTTABLE(BTUs)
EMPTY_COPYDATA(BTUs)
EMPTY_DELETEDATA(BTUs)

BTUs::BTUs() : Energy()
{
    STANDARD_CONSTRUCTOR()
}

BTUs::BTUs(const double value) : Energy(value)
{
    STANDARD_CONSTRUCTOR()
}

BTUs::BTUs(const Energy& value) : Energy()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}

