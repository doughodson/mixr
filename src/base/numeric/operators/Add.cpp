
#include "mixr/base/numeric/operators/Add.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Add, "+")
EMPTY_SLOTTABLE(Add)
EMPTY_DELETEDATA(Add)
EMPTY_COPYDATA(Add)

Add::Add()
{
   STANDARD_CONSTRUCTOR()
}

void Add::operation()
{
    val = val + n2;
}

}
}
