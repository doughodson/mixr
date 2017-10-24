
#ifndef __mixr_models_ArmoredVehicle_H__
#define __mixr_models_ArmoredVehicle_H__

#include "mixr/models/player/ground/GroundVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: ArmoredVehicle
// Factory name: ArmoredVehicle
//------------------------------------------------------------------------------
class ArmoredVehicle : public GroundVehicle
{
   DECLARE_SUBCLASS(ArmoredVehicle, GroundVehicle)

public:
   ArmoredVehicle();
};

}
}

#endif
