
#ifndef __mixr_models_common_GroundStationRadar_HPP__
#define __mixr_models_common_GroundStationRadar_HPP__

#include "mixr/models/player/ground/IGroundStation.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class GroundStationRadar
// Factory name: GroundStationRadar
//------------------------------------------------------------------------------
class GroundStationRadar final: public IGroundStation
{
   DECLARE_SUBCLASS(GroundStationRadar, IGroundStation)

public:
   GroundStationRadar();
};

}
}

#endif
