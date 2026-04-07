
#ifndef __mixr_models_common_UnmannedSpaceVehicle_HPP__
#define __mixr_models_common_UnmannedSpaceVehicle_HPP__

#include "mixr/models/player/space/ISpaceVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: UnmannedSpaceVehicle
// Factory name: UnmannedSpaceVehicle
//------------------------------------------------------------------------------
class UnmannedSpaceVehicle : public ISpaceVehicle
{
   DECLARE_SUBCLASS(UnmannedSpaceVehicle, ISpaceVehicle)

public:
   UnmannedSpaceVehicle();

protected:
   void dynamics(const double  dt = 0.0) override;
};

}
}

#endif
