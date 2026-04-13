
#ifndef __mixr_models_common_Aircraft_HPP__
#define __mixr_models_common_Aircraft_HPP__

#include "mixr/models/player/air/IAircraft.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Aircraft
// Factory name: Aircraft
//------------------------------------------------------------------------------
class Aircraft final: public IAircraft
{
    DECLARE_SUBCLASS(Aircraft, IAircraft)

public:
    Aircraft();
};

}
}

#endif
