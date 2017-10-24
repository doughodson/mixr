
#ifndef __mixr_models_Artillery_H__
#define __mixr_models_Artillery_H__

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
