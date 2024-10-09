
#ifndef __mixr_models_common_Artillery_HPP__
#define __mixr_models_common_Artillery_HPP__

#include "mixr/models/player/ground/GroundVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Artillery
// Factory name: Artillery
//------------------------------------------------------------------------------
class Artillery : public GroundVehicle
{
   DECLARE_SUBCLASS(Artillery, GroundVehicle)

public:
   Artillery();
};

}
}

#endif
