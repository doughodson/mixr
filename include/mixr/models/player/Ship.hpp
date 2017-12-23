
#ifndef __mixr_models_Ship_H__
#define __mixr_models_Ship_H__

#include "mixr/models/player/Player.hpp"

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
