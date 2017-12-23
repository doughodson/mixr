
#ifndef __mixr_models_Sam_H__
#define __mixr_models_Sam_H__

#include "mixr/models/player/weapon/Missile.hpp"

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
