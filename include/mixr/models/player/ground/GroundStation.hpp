

#ifndef __mixr_models_common_GroundStation_HPP__
#define __mixr_models_common_GroundStation_HPP__

#include "mixr/models/player/ground/IGroundStation.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class GroundStation
// Factory name: GroundStation
//------------------------------------------------------------------------------
class GroundStation final: public IGroundStation
{
   DECLARE_SUBCLASS(GroundStation, IGroundStation)

public:
   GroundStation();
};

}
}

#endif
