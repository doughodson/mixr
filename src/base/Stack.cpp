
#include "mixr/base/Stack.hpp"
#include <stdlib.h>

namespace mixr {
namespace base {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Stack, "Stack")

EMPTY_COPYDATA(Stack)
EMPTY_DELETEDATA(Stack)

Stack::Stack()
{
   STANDARD_CONSTRUCTOR()
}

}
}
