
#ifndef __mixr_models_common_Aircraft_HPP__
#define __mixr_models_common_Aircraft_HPP__

#include "mixr/models/player/air/AirVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Aircraft
// Factory name: Aircraft
//------------------------------------------------------------------------------
class Aircraft : public AirVehicle
{
    DECLARE_SUBCLASS(Aircraft, AirVehicle)

public:
    Aircraft();
};

}
}

#endif
