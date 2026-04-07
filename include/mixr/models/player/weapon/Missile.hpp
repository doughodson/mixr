
#ifndef __mixr_models_common_Missile_HPP__
#define __mixr_models_common_Missile_HPP__

#include "mixr/models/player/weapon/IMissile.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Missile
// Description: Concrete class for a missile
//------------------------------------------------------------------------------
class Missile final: public IMissile
{
    DECLARE_SUBCLASS(Missile, IMissile)

public:
    Missile();
};

}
}


#endif
