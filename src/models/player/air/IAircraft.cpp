
#include "mixr/models/player/air/IAircraft.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IAircraft, "IAircraft")
EMPTY_SLOTTABLE(IAircraft)
EMPTY_DELETEDATA(IAircraft)
EMPTY_COPYDATA(IAircraft)

IAircraft::IAircraft()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GenericAircraft");
    setType_old(&generic);
    setType("GenericAircraft");
}

}
}
