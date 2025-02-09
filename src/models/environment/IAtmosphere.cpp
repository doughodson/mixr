
#include "mixr/models/environment/IAtmosphere.hpp"

namespace mixr {
namespace models {

IMPLEMENT_ABSTRACT_SUBCLASS(IAtmosphere, "IAtmosphere")
EMPTY_SLOTTABLE(IAtmosphere)
EMPTY_COPYDATA(IAtmosphere)
EMPTY_DELETEDATA(IAtmosphere)

IAtmosphere::IAtmosphere()
{
   STANDARD_CONSTRUCTOR()
}

}
}
