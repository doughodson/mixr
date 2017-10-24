
#include "mixr/models/player/ground/WheeledVehicle.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(WheeledVehicle, "WheeledVehicle")
EMPTY_SLOTTABLE(WheeledVehicle)
EMPTY_DELETEDATA(WheeledVehicle)
EMPTY_COPYDATA(WheeledVehicle)

WheeledVehicle::WheeledVehicle()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GenericWheeledVehicle");
    setType(&generic);
}

}
}
