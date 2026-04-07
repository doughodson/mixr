
#ifndef __mixr_models_common_IAircraft_HPP__
#define __mixr_models_common_IAircraft_HPP__

#include "mixr/models/player/air/IAirVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: IAircraft
// Description: Interface for all aircraft
//------------------------------------------------------------------------------
class IAircraft : public IAirVehicle
{
    DECLARE_SUBCLASS(IAircraft, IAirVehicle)

public:
    IAircraft();
};

}
}

#endif
