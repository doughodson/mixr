
#ifndef __mixr_models_common_Aircraft_HPP__
#define __mixr_models_common_Aircraft_HPP__

#include "mixr/models/player/air/IAirVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Aircraft
// Factory name: Aircraft
//------------------------------------------------------------------------------
class Aircraft final: public IAirVehicle
{
    DECLARE_SUBCLASS(Aircraft, IAirVehicle)

public:
    Aircraft();
};

}
}

#endif
