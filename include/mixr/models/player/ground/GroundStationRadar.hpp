
#ifndef __mixr_models_GroundStationRadar_H__
#define __mixr_models_GroundStationRadar_H__

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
