
#include "mixr/models/player/space/MannedSpaceVehicle.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(MannedSpaceVehicle,"MannedSpaceVehicle")
EMPTY_SLOTTABLE(MannedSpaceVehicle)
EMPTY_COPYDATA(MannedSpaceVehicle)
EMPTY_DELETEDATA(MannedSpaceVehicle)

MannedSpaceVehicle::MannedSpaceVehicle()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GenericMannedSpaceVehicle");
    setType(&generic);
}

}
}
