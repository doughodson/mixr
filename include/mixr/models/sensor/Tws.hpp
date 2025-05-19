
#ifndef __mixr_models_common_Tws_HPP__
#define __mixr_models_common_Tws_HPP__

#include "mixr/models/system/IRadar.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Tws
// Description: Concrete Simple, Track-While-Scan Mode Radar
//------------------------------------------------------------------------------
class Tws : public IRadar
{
    DECLARE_SUBCLASS(Tws, IRadar)

public:
    Tws();
};

}
}

#endif
