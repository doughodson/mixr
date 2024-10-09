
#ifndef __mixr_models_common_UnmannedAirVehicle_HPP__
#define __mixr_models_common_UnmannedAirVehicle_HPP__

#include "mixr/models/player/air/AirVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: UnmannedAirVehicle
// Factory name: UnmannedAirVehicle
//------------------------------------------------------------------------------
class UnmannedAirVehicle : public AirVehicle
{
    DECLARE_SUBCLASS(UnmannedAirVehicle, AirVehicle)

public:
    UnmannedAirVehicle();
};

}
}

#endif
