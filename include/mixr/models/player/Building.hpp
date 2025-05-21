
#ifndef __mixr_models_Building_HPP__
#define __mixr_models_Building_HPP__

#include "mixr/models/player/IPlayer.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Building
// Description: Generic building
// Factory name: Building
//------------------------------------------------------------------------------
class Building : public IPlayer
{
    DECLARE_SUBCLASS(Building, IPlayer)

public:
    Building();

    unsigned int getMajorType() const override;
};

}
}

#endif
