
#include "mixr/linearsystem/Limit11.hpp"
#include <iostream>

namespace mixr {
namespace linearsystem {

IMPLEMENT_SUBCLASS(Limit11, "Limit11")
EMPTY_SLOTTABLE(Limit11)
EMPTY_COPYDATA(Limit11)
EMPTY_DELETEDATA(Limit11)

Limit11::Limit11() : LimitFunc(1,1)
{
   STANDARD_CONSTRUCTOR()
}

}
}
