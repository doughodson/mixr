
#ifndef __mixr_models_Tank_H__
#define __mixr_models_Tank_H__

#include "mixr/models/player/ground/GroundVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Tank
// Factory name: Tank
//------------------------------------------------------------------------------
class Tank : public GroundVehicle
{
   DECLARE_SUBCLASS(Tank, GroundVehicle)

public:
   Tank();
};

}
}

#endif
