
#include "mixr/simulation/AbstractOtw.hpp"

namespace mixr {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(AbstractOtw, "AbstractOtw")

EMPTY_SLOTTABLE(AbstractOtw)
EMPTY_COPYDATA(AbstractOtw)
EMPTY_DELETEDATA(AbstractOtw)

AbstractOtw::AbstractOtw()
{
   STANDARD_CONSTRUCTOR()
}

}
}
