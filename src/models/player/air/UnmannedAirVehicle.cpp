
#include "mixr/models/player/air/UnmannedAirVehicle.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(UnmannedAirVehicle, "UnmannedAirVehicle")
EMPTY_SLOTTABLE(UnmannedAirVehicle)
EMPTY_DELETEDATA(UnmannedAirVehicle)
EMPTY_COPYDATA(UnmannedAirVehicle)

UnmannedAirVehicle::UnmannedAirVehicle()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GenericUnmannedAirVehicle");
    setType(&generic);
}

}
}
