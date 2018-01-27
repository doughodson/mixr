
#include "mixr/base/units/force/Newtons.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Newtons, "Newtons")
EMPTY_SLOTTABLE(Newtons)
EMPTY_COPYDATA(Newtons)
EMPTY_DELETEDATA(Newtons)

Newtons::Newtons() : Force()
{
    STANDARD_CONSTRUCTOR()
}

Newtons::Newtons(const double value) : Force(value)
{
    STANDARD_CONSTRUCTOR()
}

Newtons::Newtons(const Force& value) : Force()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}

