
#ifndef __mixr_models_common_UnmannedAirVehicle_HPP__
#define __mixr_models_common_UnmannedAirVehicle_HPP__

#include "mixr/models/player/air/IAirVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: UnmannedAirVehicle
// Factory name: UnmannedAirVehicle
//------------------------------------------------------------------------------
class UnmannedAirVehicle final: public IAirVehicle
{
    DECLARE_SUBCLASS(UnmannedAirVehicle, IAirVehicle)

public:
    UnmannedAirVehicle();
};

}
}

#endif
