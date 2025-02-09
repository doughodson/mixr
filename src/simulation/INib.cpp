
#include "mixr/simulation/INib.hpp"

namespace mixr {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(INib, "INib")

EMPTY_SLOTTABLE(INib)
EMPTY_COPYDATA(INib)
EMPTY_DELETEDATA(INib)

INib::INib()
{
   STANDARD_CONSTRUCTOR()
}

}
}
