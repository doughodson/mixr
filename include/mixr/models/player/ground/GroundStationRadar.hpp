
#ifndef __mixr_models_common_GroundStationRadar_HPP__
#define __mixr_models_common_GroundStationRadar_HPP__

#include "mixr/models/player/ground/GroundStation.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class GroundStationRadar
// Factory name: GroundStationRadar
//------------------------------------------------------------------------------
class GroundStationRadar : public GroundStation
{
   DECLARE_SUBCLASS(GroundStationRadar, GroundStation)

public:
   GroundStationRadar();
};

}
}

#endif
