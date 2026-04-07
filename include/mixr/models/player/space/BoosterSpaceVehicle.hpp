
#ifndef __mixr_models_common_BoosterSpaceVehicle_HPP__
#define __mixr_models_common_BoosterSpaceVehicle_HPP__

#include "mixr/models/player/space/ISpaceVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: BoosterSpaceVehicle
// Factory name: BoosterSpaceVehicle
//------------------------------------------------------------------------------
class BoosterSpaceVehicle : public ISpaceVehicle
{
   DECLARE_SUBCLASS(BoosterSpaceVehicle, ISpaceVehicle)

public:
   BoosterSpaceVehicle();
};

}
}

#endif
