
#include "mixr/base/Stack.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Stack, "Stack")
EMPTY_SLOTTABLE(Stack)
EMPTY_COPYDATA(Stack)
EMPTY_DELETEDATA(Stack)

Stack::Stack()
{
   STANDARD_CONSTRUCTOR()
}

}
}
