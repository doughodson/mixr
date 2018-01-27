
#include "mixr/base/numeric/operators/Subtract.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Subtract, "-")
EMPTY_SLOTTABLE(Subtract)
EMPTY_COPYDATA(Subtract)
EMPTY_DELETEDATA(Subtract)

Subtract::Subtract()
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// operation() -- the operations
//------------------------------------------------------------------------------
void Subtract::operation()
{
    val = val - n2;
}

}
}
