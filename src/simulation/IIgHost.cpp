
#include "mixr/simulation/IIgHost.hpp"

namespace mixr {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(IIgHost, "IIgHost")

EMPTY_SLOTTABLE(IIgHost)
EMPTY_COPYDATA(IIgHost)
EMPTY_DELETEDATA(IIgHost)

IIgHost::IIgHost()
{
   STANDARD_CONSTRUCTOR()
}

}
}
