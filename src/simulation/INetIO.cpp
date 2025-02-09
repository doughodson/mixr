
#include "mixr/simulation/INetIO.hpp"

namespace mixr {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(INetIO, "INetIO")

EMPTY_SLOTTABLE(INetIO)
EMPTY_COPYDATA(INetIO)
EMPTY_DELETEDATA(INetIO)

INetIO::INetIO()
{
   STANDARD_CONSTRUCTOR()
}

}
}
