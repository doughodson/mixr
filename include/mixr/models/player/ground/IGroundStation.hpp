
#ifndef __mixr_models_common_IGroundStation_HPP__
#define __mixr_models_common_IGroundStation_HPP__

#include "mixr/models/player/ground/IGroundVehicle.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class IGroundStation
//
// Description: Interface for all ground stations
//------------------------------------------------------------------------------
class IGroundStation : public IGroundVehicle
{
   DECLARE_SUBCLASS(IGroundStation, IGroundVehicle)

public:
   IGroundStation();
};

}
}

#endif
