
#ifndef __mixr_models_common_Sam_HPP__
#define __mixr_models_common_Sam_HPP__

#include "mixr/models/common/player/weapon/Missile.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Sam
//
// Description: Base class for Surface to Air (SAM) Missiles
//
// Factory name: Sam
//------------------------------------------------------------------------------
class Sam : public Missile
{
    DECLARE_SUBCLASS(Sam, Missile)

public:
    Sam();

    const char* getDescription() const override;
    const char* getNickname() const override;
    int getCategory() const override;
};

}
}

#endif
