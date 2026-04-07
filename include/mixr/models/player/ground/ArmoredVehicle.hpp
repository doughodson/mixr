
#ifndef __mixr_models_common_ArmoredVehicle_HPP__
#define __mixr_models_common_ArmoredVehicle_HPP__

#include "mixr/models/player/ground/IGroundVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: ArmoredVehicle
// Factory name: ArmoredVehicle
//------------------------------------------------------------------------------
class ArmoredVehicle final: public IGroundVehicle
{
   DECLARE_SUBCLASS(ArmoredVehicle, IGroundVehicle)

public:
   ArmoredVehicle();
};

}
}

#endif
