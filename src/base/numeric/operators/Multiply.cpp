
#include "mixr/base/numeric/operators/Multiply.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Multiply, "*")
EMPTY_SLOTTABLE(Multiply)
EMPTY_COPYDATA(Multiply)
EMPTY_DELETEDATA(Multiply)

Multiply::Multiply()
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// operation() -- the operations
//------------------------------------------------------------------------------
void Multiply::operation()
{
    val = val * n2;
}

}
}
