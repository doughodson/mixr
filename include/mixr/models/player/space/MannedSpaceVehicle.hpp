
#ifndef __mixr_models_common_MannedSpaceVehicle_HPP__
#define __mixr_models_common_MannedSpaceVehicle_HPP__

#include "mixr/models/player/space/ISpaceVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: MannedSpaceVehicle
// Factory name: MannedSpaceVehicle
//------------------------------------------------------------------------------
class MannedSpaceVehicle : public ISpaceVehicle
{
   DECLARE_SUBCLASS(MannedSpaceVehicle, ISpaceVehicle)

public:
   MannedSpaceVehicle();
};

}
}

#endif
