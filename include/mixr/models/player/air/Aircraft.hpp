
#ifndef __mixr_models_Aircraft_H__
#define __mixr_models_Aircraft_H__

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
