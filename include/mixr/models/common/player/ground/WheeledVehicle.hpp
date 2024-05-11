
#ifndef __mixr_models_common_WheeledVehicle_HPP__
#define __mixr_models_common_WheeledVehicle_HPP__

#include "mixr/models/common/player/ground/GroundVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: WheeledVehicle
// Factory name: WheeledVehicle
//------------------------------------------------------------------------------
class WheeledVehicle : public GroundVehicle
{
   DECLARE_SUBCLASS(WheeledVehicle, GroundVehicle)

public:
   WheeledVehicle();
};

}
}

#endif
