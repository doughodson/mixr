
#include "mixr/simulation/AbstractIgHost.hpp"

namespace mixr {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(AbstractIgHost, "AbstractIgHost")

EMPTY_SLOTTABLE(AbstractIgHost)
EMPTY_COPYDATA(AbstractIgHost)
EMPTY_DELETEDATA(AbstractIgHost)

AbstractIgHost::AbstractIgHost()
{
   STANDARD_CONSTRUCTOR()
}

}
}
