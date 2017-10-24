
#ifndef __mixr_models_WheeledVehicle_H__
#define __mixr_models_WheeledVehicle_H__

#include "mixr/models/player/ground/GroundVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: WheeledVehicle
// Factory name: WheeledVehicle
//------------------------------------------------------------------------------
class WheeledVehicle : public GroundVehicle
{
   DECLARE_SUBCLASS(WheeledVehicle, GroundVehicle)

public:
   WheeledVehicle();
};

}
}

#endif
