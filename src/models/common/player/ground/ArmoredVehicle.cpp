
#include "mixr/models/common/player/ground/ArmoredVehicle.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(ArmoredVehicle, "ArmoredVehicle")
EMPTY_SLOTTABLE(ArmoredVehicle)
EMPTY_DELETEDATA(ArmoredVehicle)
EMPTY_COPYDATA(ArmoredVehicle)

ArmoredVehicle::ArmoredVehicle()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GenericArmoredVehicle");
    setType_old(&generic);
    setType("GenericArmoredVehicle");
}

}
}
