
#ifndef __mixr_models_Radar_HPP__
#define __mixr_models_Radar_HPP__

#include "mixr/models/system/IRadar.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Radar
// Description: Concrete radar
// Factory name: Radar
//------------------------------------------------------------------------------
class Radar final: public IRadar
{
    DECLARE_SUBCLASS(Radar, IRadar)

public:
    Radar();
};

}
}

#endif
