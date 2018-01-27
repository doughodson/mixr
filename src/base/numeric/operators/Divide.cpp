
#include "mixr/base/numeric/operators/Divide.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Divide, "/")
EMPTY_SLOTTABLE(Divide)
EMPTY_COPYDATA(Divide)
EMPTY_DELETEDATA(Divide)

Divide::Divide()
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// operation() -- the operations
//------------------------------------------------------------------------------
void Divide::operation()
{
    if (n2 != 0.0) val = (val / n2);  // only if non-zero divisor
}

}
}
