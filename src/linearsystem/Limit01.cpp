
#include "mixr/linearsystem/Limit01.hpp"
#include <iostream>

namespace mixr {
namespace linearsystem {

IMPLEMENT_SUBCLASS(Limit01, "Limit01")
EMPTY_SLOTTABLE(Limit01)
EMPTY_COPYDATA(Limit01)
EMPTY_DELETEDATA(Limit01)

Limit01::Limit01() : LimitFunc(0,1)
{
   STANDARD_CONSTRUCTOR()
}

}
}
