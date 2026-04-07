
#ifndef __mixr_models_common_Artillery_HPP__
#define __mixr_models_common_Artillery_HPP__

#include "mixr/models/player/ground/IGroundVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Artillery
// Factory name: Artillery
//------------------------------------------------------------------------------
class Artillery final: public IGroundVehicle
{
   DECLARE_SUBCLASS(Artillery, IGroundVehicle)

public:
   Artillery();
};

}
}

#endif
