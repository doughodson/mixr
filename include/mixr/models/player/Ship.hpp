
#ifndef __mixr_models_common_Ship_HPP__
#define __mixr_models_common_Ship_HPP__

#include "mixr/models/player/IPlayer.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Ship
// Description: Generic ship model
// Factory name: Ship
//------------------------------------------------------------------------------
class Ship : public IPlayer
{
    DECLARE_SUBCLASS(Ship, IPlayer)

public:
    Ship();

    unsigned int getMajorType() const override;
};

}
}

#endif
