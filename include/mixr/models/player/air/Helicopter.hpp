
#ifndef __mixr_models_Helicopter_H__
#define __mixr_models_Helicopter_H__

#include "mixr/models/player/air/AirVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Helicopter
// Factory name: Helicopter
//------------------------------------------------------------------------------
class Helicopter : public AirVehicle
{
    DECLARE_SUBCLASS(Helicopter, AirVehicle)

public:
    Helicopter();
};

}
}

#endif
