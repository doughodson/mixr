
#ifndef __mixr_models_common_UnmannedSpaceVehicle_HPP__
#define __mixr_models_common_UnmannedSpaceVehicle_HPP__

#include "mixr/models/player/space/SpaceVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: UnmannedSpaceVehicle
// Factory name: UnmannedSpaceVehicle
//------------------------------------------------------------------------------
class UnmannedSpaceVehicle : public SpaceVehicle
{
   DECLARE_SUBCLASS(UnmannedSpaceVehicle, SpaceVehicle)

public:
   UnmannedSpaceVehicle();

protected:
   void dynamics(const double  dt = 0.0) override;
};

}
}

#endif
