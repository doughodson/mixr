
#ifndef __mixr_models_common_WheeledVehicle_HPP__
#define __mixr_models_common_WheeledVehicle_HPP__

#include "mixr/models/player/ground/IGroundVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: WheeledVehicle
// Factory name: WheeledVehicle
//------------------------------------------------------------------------------
class WheeledVehicle : public IGroundVehicle
{
   DECLARE_SUBCLASS(WheeledVehicle, IGroundVehicle)

public:
   WheeledVehicle();
};

}
}

#endif
