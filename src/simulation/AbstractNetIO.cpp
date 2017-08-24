
#include "mixr/simulation/AbstractNetIO.hpp"

namespace mixr {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(AbstractNetIO, "AbstractNetIO")

EMPTY_SLOTTABLE(AbstractNetIO)
EMPTY_COPYDATA(AbstractNetIO)
EMPTY_DELETEDATA(AbstractNetIO)

AbstractNetIO::AbstractNetIO()
{
   STANDARD_CONSTRUCTOR()
}

}
}
