
#ifndef __mixr_models_GroundStationUav_H__
#define __mixr_models_GroundStationUav_H__

#include "mixr/models/player/ground/GroundStation.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class GroundStationUav
// Factory name: GroundStationUav
//------------------------------------------------------------------------------
class GroundStationUav : public GroundStation
{
   DECLARE_SUBCLASS(GroundStationUav, GroundStation)

public:
   GroundStationUav();
};

}
}

#endif
