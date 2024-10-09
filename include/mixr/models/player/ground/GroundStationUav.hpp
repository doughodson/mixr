
#ifndef __mixr_models_common_GroundStationUav_HPP__
#define __mixr_models_common_GroundStationUav_HPP__

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
