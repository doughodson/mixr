
#ifndef __mixr_models_MannedSpaceVehicle_H__
#define __mixr_models_MannedSpaceVehicle_H__

#include "mixr/models/player/space/SpaceVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: MannedSpaceVehicle
// Factory name: MannedSpaceVehicle
//------------------------------------------------------------------------------
class MannedSpaceVehicle : public SpaceVehicle
{
   DECLARE_SUBCLASS(MannedSpaceVehicle, SpaceVehicle)

public:
   MannedSpaceVehicle();
};

}
}

#endif
