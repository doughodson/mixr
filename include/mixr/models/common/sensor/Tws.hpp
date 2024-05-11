
#ifndef __mixr_models_common_Tws_HPP__
#define __mixr_models_common_Tws_HPP__

#include "mixr/models/common/system/Radar.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Tws
// Description: Simple, Track-While-Scan Mode Radar
//------------------------------------------------------------------------------
class Tws : public Radar
{
    DECLARE_SUBCLASS(Tws, Radar)

public:
    Tws();
};

}
}

#endif
