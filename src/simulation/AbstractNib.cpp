
#include "mxrp/simulation/AbstractNib.hpp"

namespace mxrp {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(AbstractNib, "AbstractNib")

EMPTY_SLOTTABLE(AbstractNib)
EMPTY_COPYDATA(AbstractNib)
EMPTY_DELETEDATA(AbstractNib)
EMPTY_SERIALIZER(AbstractNib)

AbstractNib::AbstractNib()
{
   STANDARD_CONSTRUCTOR()
}

}
}
