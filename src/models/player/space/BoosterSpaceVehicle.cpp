
#include "mixr/models/player/space/BoosterSpaceVehicle.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(BoosterSpaceVehicle,"BoosterSpaceVehicle")
EMPTY_SLOTTABLE(BoosterSpaceVehicle)
EMPTY_COPYDATA(BoosterSpaceVehicle)
EMPTY_DELETEDATA(BoosterSpaceVehicle)

BoosterSpaceVehicle::BoosterSpaceVehicle()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GenericBoosterSpaceVehicle");
    setType(&generic);
}

}
}
