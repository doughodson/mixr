
#ifndef __mixr_models_Building_H__
#define __mixr_models_Building_H__

#include "mixr/models/player/Player.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Building
// Description: Generic building
// Factory name: Building
//------------------------------------------------------------------------------
class Building : public Player
{
    DECLARE_SUBCLASS(Building, Player)

public:
    Building();

    unsigned int getMajorType() const override;
};

}
}

#endif
