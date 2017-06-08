
#ifndef __mxrp_models_Tws_H__
#define __mxrp_models_Tws_H__

#include "mxrp/models/system/Radar.hpp"

namespace mxrp {
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
