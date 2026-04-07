
#ifndef __mixr_models_common_Helicopter_HPP__
#define __mixr_models_common_Helicopter_HPP__

#include "mixr/models/player/air/IAirVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Helicopter
// Factory name: Helicopter
//------------------------------------------------------------------------------
class Helicopter final: public IAirVehicle
{
    DECLARE_SUBCLASS(Helicopter, IAirVehicle)

public:
    Helicopter();
};

}
}

#endif
