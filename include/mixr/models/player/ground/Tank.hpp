
#ifndef __mixr_models_common_Tank_HPP__
#define __mixr_models_common_Tank_HPP__

#include "mixr/models/player/ground/IGroundVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Tank
// Factory name: Tank
//------------------------------------------------------------------------------
class Tank : public IGroundVehicle
{
   DECLARE_SUBCLASS(Tank, IGroundVehicle)

public:
   Tank();
};

}
}

#endif
