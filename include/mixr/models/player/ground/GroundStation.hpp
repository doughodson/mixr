
#ifndef __mixr_models_common_GroundStation_HPP__
#define __mixr_models_common_GroundStation_HPP__

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
