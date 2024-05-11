
#ifndef __mixr_models_common_BoosterSpaceVehicle_HPP__
#define __mixr_models_common_BoosterSpaceVehicle_HPP__

#include "mixr/models/common/player/space/SpaceVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: BoosterSpaceVehicle
// Factory name: BoosterSpaceVehicle
//------------------------------------------------------------------------------
class BoosterSpaceVehicle : public SpaceVehicle
{
   DECLARE_SUBCLASS(BoosterSpaceVehicle, SpaceVehicle)

public:
   BoosterSpaceVehicle();
};

}
}

#endif
