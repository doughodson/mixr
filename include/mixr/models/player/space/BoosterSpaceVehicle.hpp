
#ifndef __mixr_models_BoosterSpaceVehicle_H__
#define __mixr_models_BoosterSpaceVehicle_H__

#include "mixr/models/player/space/SpaceVehicle.hpp"

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
