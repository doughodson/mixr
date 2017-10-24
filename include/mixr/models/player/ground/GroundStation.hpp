
#ifndef __mixr_models_GroundStation_H__
#define __mixr_models_GroundStation_H__

#include "mixr/models/player/ground/GroundVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class GroundStation
// Factory name: GroundStation
//------------------------------------------------------------------------------
class GroundStation : public GroundVehicle
{
   DECLARE_SUBCLASS(GroundStation, GroundVehicle)

public:
   GroundStation();
};

}
}

#endif
