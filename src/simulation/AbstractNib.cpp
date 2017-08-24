
#include "mixr/simulation/AbstractNib.hpp"

namespace mixr {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(AbstractNib, "AbstractNib")

EMPTY_SLOTTABLE(AbstractNib)
EMPTY_COPYDATA(AbstractNib)
EMPTY_DELETEDATA(AbstractNib)

AbstractNib::AbstractNib()
{
   STANDARD_CONSTRUCTOR()
}

}
}
