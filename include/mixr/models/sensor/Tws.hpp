
#ifndef __mixr_models_Tws_H__
#define __mixr_models_Tws_H__

#include "mixr/models/system/Radar.hpp"

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
