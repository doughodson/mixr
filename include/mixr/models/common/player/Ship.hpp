
#ifndef __mixr_models_common_Ship_HPP__
#define __mixr_models_common_Ship_HPP__

#include "mixr/models/common/player/Player.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Ship
// Description: Generic ship model
// Factory name: Ship
//------------------------------------------------------------------------------
class Ship : public Player
{
    DECLARE_SUBCLASS(Ship, Player)

public:
    Ship();

    unsigned int getMajorType() const override;
};

}
}

#endif
