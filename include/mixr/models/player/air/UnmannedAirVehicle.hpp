
#ifndef __mixr_models_UnmannedAirVehicle_H__
#define __mixr_models_UnmannedAirVehicle_H__

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
