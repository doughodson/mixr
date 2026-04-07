
#ifndef __mixr_models_common_GroundStationUav_HPP__
#define __mixr_models_common_GroundStationUav_HPP__

#include "mixr/models/player/ground/IGroundStation.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class GroundStationUav
// Factory name: GroundStationUav
//------------------------------------------------------------------------------
class GroundStationUav final: public IGroundStation
{
   DECLARE_SUBCLASS(GroundStationUav, IGroundStation)

public:
   GroundStationUav();
};

}
}

#endif
